import { getWebGLContext, initShaders } from "../gllib/cuon-utils";
import * as fs from "fs";
import * as path from "path";
import AssetManager, { AssetType } from "../../AssetManager";

function initVertexBuffers(gl) {
    // Mapping webgl coordinates(-0.5, -0.5 -> 0.5, 0.5) to normal coordinates(0.0, 0.0 -> 1.0, 1.0).
    // x, y, texcoord1, texcoord2
    let verticesTexCoords = new Float32Array([
        -0.5, 0.5, 0.0, 1.0,
        -0.5, -0.5, 0.0, 0.0,
        0.5, 0.5, 1.0, 1.0,           // Point 2.
        0.5, -0.5, 1.0, 0.0             // Point 3.
    ]);
    let numVetices = 4;
    // Create buffer object.
    let vertexTexCoordBuffer = gl.createBuffer();
    if (!vertexTexCoordBuffer) {
        console.log("Failed to create the buffer obejct.");
        return -1;
    }
    // Binding buffer object to target.
    gl.bindBuffer(gl.ARRAY_BUFFER, vertexTexCoordBuffer);
    // Write data to buffer object.
    gl.bufferData(gl.ARRAY_BUFFER, verticesTexCoords, gl.STATIC_DRAW);
    const FSIZE = verticesTexCoords.BYTES_PER_ELEMENT;
    let aPosition = gl.getAttribLocation(gl.program, "a_Position");
    if (aPosition < 0) {
        console.log("Failed to get the storage location of a_Position");
        return -1;
    }
    gl.vertexAttribPointer(aPosition, 2, gl.FLOAT, false, FSIZE * 4, 0);
    // Connect variable aPosition to its assigned buffer object.
    gl.enableVertexAttribArray(aPosition);
    let aTexCoord = gl.getAttribLocation(gl.program, "a_TexCoord");
    if (aTexCoord < 0) {
        console.log("Failed to get the storage location of a_TexCoord.");
        return -1;
    }
    gl.vertexAttribPointer(aTexCoord, 2, gl.FLOAT, false, FSIZE * 4, FSIZE * 2);
    gl.enableVertexAttribArray(aTexCoord);
    return numVetices;
}

function initTextures(gl, numVetices) {
    let texture = gl.createTexture();       // Create texture object.
    if (!texture) {
        console.log("Failed to create the texture obejct.");
        return false;
    }
    let uSampler = gl.getUniformLocation(gl.program, "u_Sampler");
    if (!uSampler) {
        console.log("Failed to get the storage location of u_Sampler.");
        return false;
    }
    let imgObj = new Image();       // Create image object.
    if (!imgObj) {
        console.log("Failed to create the image obejct.");
        return false;
    }
    // Register response function on loading event.
    imgObj.onload = () => {
        loadTexture(gl, numVetices, texture, uSampler, imgObj);
    }
    // Add this line to allow access of cross-origin.
    // imgObj.crossOrigin = "anonymous";
    imgObj.src = AssetManager.getAssetPath("pic", "sky.jpg");
    return true;
}

function loadTexture(gl, numVetices, texture, uSampler, imgObj) {
    gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, 1);          // Invert y axis for texture image.
    // Enable number 0 texture unit.
    gl.activeTexture(gl.TEXTURE0);
    // Binding texture object to target.
    gl.bindTexture(gl.TEXTURE_2D, texture);
    // Configure texture parameters.
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
    // Configure texture image.
    gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGB, gl.RGB, gl.UNSIGNED_BYTE, imgObj);
    // Pass number 0 texture to shader.
    gl.uniform1i(uSampler, 0);
    gl.clear(gl.COLOR_BUFFER_BIT);   // Clear <canvas>
    gl.drawArrays(gl.TRIANGLE_STRIP, 0, numVetices);
}

export function TexturedQuad() {
    const VSHADER_SOURCE = AssetManager.loadSync(AssetManager.getAssetPath("shaders", "TexturedQuad.vert"), AssetType.PLAIN_TEXT).data;
    const FSHADER_SOURCE = AssetManager.loadSync(AssetManager.getAssetPath("shaders", "TexturedQuad.frag"), AssetType.PLAIN_TEXT).data;
    // console.log(VSHADER_SOURCE.data);
    // console.log(FSHADER_SOURCE.data);
    const canvas = document.getElementById("webgl");
    const gl = getWebGLContext(canvas);
    if (!gl) {
        console.log("Failed to get the rendering context of WebGL!");
        return;
    }
    // Initialize shaders.
    if (!initShaders(gl, VSHADER_SOURCE, FSHADER_SOURCE)) {
        console.log("Failed to initilize shaders.");
        return;
    }
    let numVetices = initVertexBuffers(gl);
    if (numVetices < 0) {
        console.log("Failed to set the positions of the vetices.");
        return;
    }
    gl.clearColor(0.0, 0.0, 0.0, 1.0);
    if (!initTextures(gl, numVetices)) {
        console.log("Failed to initilize textures.");
        return;
    }
}