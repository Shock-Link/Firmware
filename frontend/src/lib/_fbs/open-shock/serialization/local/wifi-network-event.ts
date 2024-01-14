// automatically generated by the FlatBuffers compiler, do not modify

/* eslint-disable @typescript-eslint/no-unused-vars, @typescript-eslint/no-explicit-any, @typescript-eslint/no-non-null-assertion */

import * as flatbuffers from 'flatbuffers';

import { WifiNetwork } from '../../../open-shock/serialization/types/wifi-network';
import { WifiNetworkEventType } from '../../../open-shock/serialization/types/wifi-network-event-type';


export class WifiNetworkEvent {
  bb: flatbuffers.ByteBuffer|null = null;
  bb_pos = 0;
  __init(i:number, bb:flatbuffers.ByteBuffer):WifiNetworkEvent {
  this.bb_pos = i;
  this.bb = bb;
  return this;
}

static getRootAsWifiNetworkEvent(bb:flatbuffers.ByteBuffer, obj?:WifiNetworkEvent):WifiNetworkEvent {
  return (obj || new WifiNetworkEvent()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

static getSizePrefixedRootAsWifiNetworkEvent(bb:flatbuffers.ByteBuffer, obj?:WifiNetworkEvent):WifiNetworkEvent {
  bb.setPosition(bb.position() + flatbuffers.SIZE_PREFIX_LENGTH);
  return (obj || new WifiNetworkEvent()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

eventType():WifiNetworkEventType {
  const offset = this.bb!.__offset(this.bb_pos, 4);
  return offset ? this.bb!.readUint8(this.bb_pos + offset) : WifiNetworkEventType.Discovered;
}

network(obj?:WifiNetwork):WifiNetwork|null {
  const offset = this.bb!.__offset(this.bb_pos, 6);
  return offset ? (obj || new WifiNetwork()).__init(this.bb!.__indirect(this.bb_pos + offset), this.bb!) : null;
}

static startWifiNetworkEvent(builder:flatbuffers.Builder) {
  builder.startObject(2);
}

static addEventType(builder:flatbuffers.Builder, eventType:WifiNetworkEventType) {
  builder.addFieldInt8(0, eventType, WifiNetworkEventType.Discovered);
}

static addNetwork(builder:flatbuffers.Builder, networkOffset:flatbuffers.Offset) {
  builder.addFieldOffset(1, networkOffset, 0);
}

static endWifiNetworkEvent(builder:flatbuffers.Builder):flatbuffers.Offset {
  const offset = builder.endObject();
  return offset;
}

}
