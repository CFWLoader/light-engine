import { getWebGLContext, initShaders } from "../gllib/cuon-utils";

export function multiPoint(vShader: string, fShader: string) {
    let canvas = document.getElementById("webgl");
    let gl = getWebGLContext(canvas) as WebGLRenderingContext;
    if (!gl) {
        console.log("Failed to get the rendering context of WebGL!");
        return;
    }
    // Initialize shaders.
    if (!initShaders(gl, vShader, fShader)) {
        console.log("Failed to initilize shaders.");
        return;
    }
    let numVetices = initVertexBuffers(gl);
    if (numVetices < 0) {
        console.log("Failed to set the positions of the vetices.");
        return;
    }
    gl.clearColor(0.0, 0.0, 0.0, 1.0);
    gl.clear(gl.COLOR_BUFFER_BIT);
    gl.drawArrays(gl.POINTS, 0, numVetices);
}

function initVertexBuffers(gl) {
    let vertices = new Float32Array([
        0.0, 0.5, -0.5, -0.5, 0.5, -0.5
    ]);
    let numVetices = vertices.length / 2;
    // Create buffer object.
    let vertexBuffer = gl.createBuffer();
    if (!vertexBuffer) {
        console.log("Failed to create the buffer obejct.");
        return -1;
    }
    // Binding buffer object to target.
    gl.bindBuffer(gl.ARRAY_BUFFER, vertexBuffer);
    // Write data to buffer object.
    gl.bufferData(gl.ARRAY_BUFFER, vertices, gl.STATIC_DRAW);
    let aPosition = gl.getAttribLocation(gl.program, "a_Position");
    if (aPosition < 0) {
        console.log("Failed to get the storage location of a_Position");
        return;
    }
    // Assign buffer object to aPosition variable.
    gl.vertexAttribPointer(aPosition, 2, gl.FLOAT, false, 0, 0);
    // Connect variable aPosition to its assigned buffer object.
    gl.enableVertexAttribArray(aPosition);
    return numVetices;
}
