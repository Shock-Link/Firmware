// automatically generated by the FlatBuffers compiler, do not modify

/* eslint-disable @typescript-eslint/no-unused-vars, @typescript-eslint/no-explicit-any, @typescript-eslint/no-non-null-assertion */

import { CaptivePortalConfig } from '../../open-shock/serialization/captive-portal-config';
import { OtaDownload } from '../../open-shock/serialization/ota-download';
import { OtaInstall } from '../../open-shock/serialization/ota-install';
import { ShockerCommandList } from '../../open-shock/serialization/shocker-command-list';


export enum ServerToDeviceMessagePayload {
  NONE = 0,
  ShockerCommandList = 1,
  CaptivePortalConfig = 2,
  OtaDownload = 3,
  OtaInstall = 4
}

export function unionToServerToDeviceMessagePayload(
  type: ServerToDeviceMessagePayload,
  accessor: (obj:CaptivePortalConfig|OtaDownload|OtaInstall|ShockerCommandList) => CaptivePortalConfig|OtaDownload|OtaInstall|ShockerCommandList|null
): CaptivePortalConfig|OtaDownload|OtaInstall|ShockerCommandList|null {
  switch(ServerToDeviceMessagePayload[type]) {
    case 'NONE': return null; 
    case 'ShockerCommandList': return accessor(new ShockerCommandList())! as ShockerCommandList;
    case 'CaptivePortalConfig': return accessor(new CaptivePortalConfig())! as CaptivePortalConfig;
    case 'OtaDownload': return accessor(new OtaDownload())! as OtaDownload;
    case 'OtaInstall': return accessor(new OtaInstall())! as OtaInstall;
    default: return null;
  }
}

export function unionListToServerToDeviceMessagePayload(
  type: ServerToDeviceMessagePayload, 
  accessor: (index: number, obj:CaptivePortalConfig|OtaDownload|OtaInstall|ShockerCommandList) => CaptivePortalConfig|OtaDownload|OtaInstall|ShockerCommandList|null, 
  index: number
): CaptivePortalConfig|OtaDownload|OtaInstall|ShockerCommandList|null {
  switch(ServerToDeviceMessagePayload[type]) {
    case 'NONE': return null; 
    case 'ShockerCommandList': return accessor(index, new ShockerCommandList())! as ShockerCommandList;
    case 'CaptivePortalConfig': return accessor(index, new CaptivePortalConfig())! as CaptivePortalConfig;
    case 'OtaDownload': return accessor(index, new OtaDownload())! as OtaDownload;
    case 'OtaInstall': return accessor(index, new OtaInstall())! as OtaInstall;
    default: return null;
  }
}
