// automatically generated by the FlatBuffers compiler, do not modify

/* eslint-disable @typescript-eslint/no-unused-vars, @typescript-eslint/no-explicit-any, @typescript-eslint/no-non-null-assertion */

import * as flatbuffers from 'flatbuffers';

import { ShockerCommand } from '../../../open-shock/serialization/gateway/shocker-command';


export class ShockerCommandList {
  bb: flatbuffers.ByteBuffer|null = null;
  bb_pos = 0;
  __init(i:number, bb:flatbuffers.ByteBuffer):ShockerCommandList {
  this.bb_pos = i;
  this.bb = bb;
  return this;
}

static getRootAsShockerCommandList(bb:flatbuffers.ByteBuffer, obj?:ShockerCommandList):ShockerCommandList {
  return (obj || new ShockerCommandList()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

static getSizePrefixedRootAsShockerCommandList(bb:flatbuffers.ByteBuffer, obj?:ShockerCommandList):ShockerCommandList {
  bb.setPosition(bb.position() + flatbuffers.SIZE_PREFIX_LENGTH);
  return (obj || new ShockerCommandList()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

commands(index: number, obj?:ShockerCommand):ShockerCommand|null {
  const offset = this.bb!.__offset(this.bb_pos, 4);
  return offset ? (obj || new ShockerCommand()).__init(this.bb!.__vector(this.bb_pos + offset) + index * 8, this.bb!) : null;
}

commandsLength():number {
  const offset = this.bb!.__offset(this.bb_pos, 4);
  return offset ? this.bb!.__vector_len(this.bb_pos + offset) : 0;
}

static startShockerCommandList(builder:flatbuffers.Builder) {
  builder.startObject(1);
}

static addCommands(builder:flatbuffers.Builder, commandsOffset:flatbuffers.Offset) {
  builder.addFieldOffset(0, commandsOffset, 0);
}

static startCommandsVector(builder:flatbuffers.Builder, numElems:number) {
  builder.startVector(8, numElems, 2);
}

static endShockerCommandList(builder:flatbuffers.Builder):flatbuffers.Offset {
  const offset = builder.endObject();
  builder.requiredField(offset, 4) // commands
  return offset;
}

static createShockerCommandList(builder:flatbuffers.Builder, commandsOffset:flatbuffers.Offset):flatbuffers.Offset {
  ShockerCommandList.startShockerCommandList(builder);
  ShockerCommandList.addCommands(builder, commandsOffset);
  return ShockerCommandList.endShockerCommandList(builder);
}
}
