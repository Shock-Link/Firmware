// automatically generated by the FlatBuffers compiler, do not modify

import * as flatbuffers from 'flatbuffers';

export class WiFiCredentials {
  bb: flatbuffers.ByteBuffer|null = null;
  bb_pos = 0;
  __init(i:number, bb:flatbuffers.ByteBuffer):WiFiCredentials {
  this.bb_pos = i;
  this.bb = bb;
  return this;
}

static getRootAsWiFiCredentials(bb:flatbuffers.ByteBuffer, obj?:WiFiCredentials):WiFiCredentials {
  return (obj || new WiFiCredentials()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

static getSizePrefixedRootAsWiFiCredentials(bb:flatbuffers.ByteBuffer, obj?:WiFiCredentials):WiFiCredentials {
  bb.setPosition(bb.position() + flatbuffers.SIZE_PREFIX_LENGTH);
  return (obj || new WiFiCredentials()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

/**
 * ID of the WiFi network credentials, used for referencing the credentials with a low memory footprint
 */
id():number {
  const offset = this.bb!.__offset(this.bb_pos, 4);
  return offset ? this.bb!.readUint8(this.bb_pos + offset) : 0;
}

/**
 * SSID of the WiFi network
 */
ssid():string|null
ssid(optionalEncoding:flatbuffers.Encoding):string|Uint8Array|null
ssid(optionalEncoding?:any):string|Uint8Array|null {
  const offset = this.bb!.__offset(this.bb_pos, 6);
  return offset ? this.bb!.__string(this.bb_pos + offset, optionalEncoding) : null;
}

/**
 * Password of the WiFi network
 */
password():string|null
password(optionalEncoding:flatbuffers.Encoding):string|Uint8Array|null
password(optionalEncoding?:any):string|Uint8Array|null {
  const offset = this.bb!.__offset(this.bb_pos, 8);
  return offset ? this.bb!.__string(this.bb_pos + offset, optionalEncoding) : null;
}

static startWiFiCredentials(builder:flatbuffers.Builder) {
  builder.startObject(3);
}

static addId(builder:flatbuffers.Builder, id:number) {
  builder.addFieldInt8(0, id, 0);
}

static addSsid(builder:flatbuffers.Builder, ssidOffset:flatbuffers.Offset) {
  builder.addFieldOffset(1, ssidOffset, 0);
}

static addPassword(builder:flatbuffers.Builder, passwordOffset:flatbuffers.Offset) {
  builder.addFieldOffset(2, passwordOffset, 0);
}

static endWiFiCredentials(builder:flatbuffers.Builder):flatbuffers.Offset {
  const offset = builder.endObject();
  return offset;
}

static createWiFiCredentials(builder:flatbuffers.Builder, id:number, ssidOffset:flatbuffers.Offset, passwordOffset:flatbuffers.Offset):flatbuffers.Offset {
  WiFiCredentials.startWiFiCredentials(builder);
  WiFiCredentials.addId(builder, id);
  WiFiCredentials.addSsid(builder, ssidOffset);
  WiFiCredentials.addPassword(builder, passwordOffset);
  return WiFiCredentials.endWiFiCredentials(builder);
}
}
