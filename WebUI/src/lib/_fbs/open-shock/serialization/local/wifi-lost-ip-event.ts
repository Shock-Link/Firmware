// automatically generated by the FlatBuffers compiler, do not modify

import * as flatbuffers from 'flatbuffers';

export class WifiLostIpEvent {
  bb: flatbuffers.ByteBuffer|null = null;
  bb_pos = 0;
  __init(i:number, bb:flatbuffers.ByteBuffer):WifiLostIpEvent {
  this.bb_pos = i;
  this.bb = bb;
  return this;
}

static getRootAsWifiLostIpEvent(bb:flatbuffers.ByteBuffer, obj?:WifiLostIpEvent):WifiLostIpEvent {
  return (obj || new WifiLostIpEvent()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

static getSizePrefixedRootAsWifiLostIpEvent(bb:flatbuffers.ByteBuffer, obj?:WifiLostIpEvent):WifiLostIpEvent {
  bb.setPosition(bb.position() + flatbuffers.SIZE_PREFIX_LENGTH);
  return (obj || new WifiLostIpEvent()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

ip():string|null
ip(optionalEncoding:flatbuffers.Encoding):string|Uint8Array|null
ip(optionalEncoding?:any):string|Uint8Array|null {
  const offset = this.bb!.__offset(this.bb_pos, 4);
  return offset ? this.bb!.__string(this.bb_pos + offset, optionalEncoding) : null;
}

static startWifiLostIpEvent(builder:flatbuffers.Builder) {
  builder.startObject(1);
}

static addIp(builder:flatbuffers.Builder, ipOffset:flatbuffers.Offset) {
  builder.addFieldOffset(0, ipOffset, 0);
}

static endWifiLostIpEvent(builder:flatbuffers.Builder):flatbuffers.Offset {
  const offset = builder.endObject();
  return offset;
}

static createWifiLostIpEvent(builder:flatbuffers.Builder, ipOffset:flatbuffers.Offset):flatbuffers.Offset {
  WifiLostIpEvent.startWifiLostIpEvent(builder);
  WifiLostIpEvent.addIp(builder, ipOffset);
  return WifiLostIpEvent.endWifiLostIpEvent(builder);
}
}
