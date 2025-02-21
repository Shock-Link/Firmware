// automatically generated by the FlatBuffers compiler, do not modify

/* eslint-disable @typescript-eslint/no-unused-vars, @typescript-eslint/no-explicit-any, @typescript-eslint/no-non-null-assertion */

import * as flatbuffers from 'flatbuffers';

import { GatewayToHubMessagePayload, unionToGatewayToHubMessagePayload, unionListToGatewayToHubMessagePayload } from '../../../open-shock/serialization/gateway/gateway-to-hub-message-payload';


export class GatewayToHubMessage {
  bb: flatbuffers.ByteBuffer|null = null;
  bb_pos = 0;
  __init(i:number, bb:flatbuffers.ByteBuffer):GatewayToHubMessage {
  this.bb_pos = i;
  this.bb = bb;
  return this;
}

static getRootAsGatewayToHubMessage(bb:flatbuffers.ByteBuffer, obj?:GatewayToHubMessage):GatewayToHubMessage {
  return (obj || new GatewayToHubMessage()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

static getSizePrefixedRootAsGatewayToHubMessage(bb:flatbuffers.ByteBuffer, obj?:GatewayToHubMessage):GatewayToHubMessage {
  bb.setPosition(bb.position() + flatbuffers.SIZE_PREFIX_LENGTH);
  return (obj || new GatewayToHubMessage()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

payloadType():GatewayToHubMessagePayload {
  const offset = this.bb!.__offset(this.bb_pos, 4);
  return offset ? this.bb!.readUint8(this.bb_pos + offset) : GatewayToHubMessagePayload.NONE;
}

payload<T extends flatbuffers.Table>(obj:any):any|null {
  const offset = this.bb!.__offset(this.bb_pos, 6);
  return offset ? this.bb!.__union(obj, this.bb_pos + offset) : null;
}

static startGatewayToHubMessage(builder:flatbuffers.Builder) {
  builder.startObject(2);
}

static addPayloadType(builder:flatbuffers.Builder, payloadType:GatewayToHubMessagePayload) {
  builder.addFieldInt8(0, payloadType, GatewayToHubMessagePayload.NONE);
}

static addPayload(builder:flatbuffers.Builder, payloadOffset:flatbuffers.Offset) {
  builder.addFieldOffset(1, payloadOffset, 0);
}

static endGatewayToHubMessage(builder:flatbuffers.Builder):flatbuffers.Offset {
  const offset = builder.endObject();
  builder.requiredField(offset, 6) // payload
  return offset;
}

static finishGatewayToHubMessageBuffer(builder:flatbuffers.Builder, offset:flatbuffers.Offset) {
  builder.finish(offset);
}

static finishSizePrefixedGatewayToHubMessageBuffer(builder:flatbuffers.Builder, offset:flatbuffers.Offset) {
  builder.finish(offset, undefined, true);
}

static createGatewayToHubMessage(builder:flatbuffers.Builder, payloadType:GatewayToHubMessagePayload, payloadOffset:flatbuffers.Offset):flatbuffers.Offset {
  GatewayToHubMessage.startGatewayToHubMessage(builder);
  GatewayToHubMessage.addPayloadType(builder, payloadType);
  GatewayToHubMessage.addPayload(builder, payloadOffset);
  return GatewayToHubMessage.endGatewayToHubMessage(builder);
}
}
