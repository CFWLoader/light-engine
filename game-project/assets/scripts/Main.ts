import { multiPoint } from "./program/MultiPoint";
import { TexturedQuad } from "./program/TextureQuad";
import AssetManager, { AssetType } from "../AssetManager";

// multiPoint(VSHADER_SOURCE, FSHADER_SOURCE);
TexturedQuad();
// const picPath = AssetManager.getAssetPath("pic", "map", "grass-single.png");
// const picBinData = AssetManager.loadSync(picPath, AssetType.TEXTURE).data;
// console.log(picBinData);
// for(let idx = 0; idx < 8; ++idx) {
//   console.log(picBinData[idx])
// }