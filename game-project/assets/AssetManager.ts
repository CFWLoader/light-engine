export const enum AssetType {
    PLAIN_TEXT = 0,
};

export function loadPlainText(fileName: string, callback?: (errCode: number, data: any) => void) {
    const xhr = new XMLHttpRequest();
    // let okStatus = document.location.protocol === "http:" ? 0 : 200;
    // console.log(okStatus);
    xhr.open('GET', fileName, true);
    xhr.overrideMimeType("text/plain;charset=utf-8");//默认为utf-8
    xhr.onload = (e) => {
        callback?.(xhr.status, xhr.responseText);
    };
    xhr.onerror = (e) => {
        callback?.(xhr.status, null);
    }
    xhr.send();
}

export function loadPlainTextSync(fileName: string): { errCode: number, data: any } {
    const xhr = new XMLHttpRequest();
    // let okStatus = document.location.protocol === "http:" ? 0 : 200;
    // console.log(okStatus);
    xhr.open('GET', fileName, false);
    xhr.overrideMimeType("text/plain;charset=utf-8");//默认为utf-8
    xhr.send();
    return { errCode: xhr.status, data: xhr.responseText };
}

export default class AssetManager {
    public static readonly ASSET_ROOT = ".";
    public static readonly sep = "/";
    public static getAssetPath(...args: string[]): string {
        const pathArr = Array.from(args);
        pathArr.unshift(AssetManager.ASSET_ROOT);
        return pathArr.join(AssetManager.sep);
    }
    public static loadPlainText(assetPath: string, callback?: (errCode: number, data: any) => void) {
        loadPlainText(assetPath, callback);
    }
    public static loadPlainTextSync(assetPath: string): { errCode: number, data: any } {
        return loadPlainTextSync(assetPath);
    }
    public static load(assetPath: string, assetType: AssetType, callback?: (errCode: number, data: any) => void) {
        switch (assetType) {
            case AssetType.PLAIN_TEXT: AssetManager.loadPlainText(assetPath, callback); break;
            default:
                console.error("Unsupported Asset Type!");
        }
    }
    public static loadSync(assetPath: string, assetType: AssetType): { errCode: number, data: any } {
        switch (assetType) {
            case AssetType.PLAIN_TEXT: return AssetManager.loadPlainTextSync(assetPath);
            default:
                console.error("Unsupported Asset Type!");
        }
    }
}