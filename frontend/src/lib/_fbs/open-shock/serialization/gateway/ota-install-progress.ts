// automatically generated by the FlatBuffers compiler, do not modify

/* eslint-disable @typescript-eslint/no-unused-vars, @typescript-eslint/no-explicit-any, @typescript-eslint/no-non-null-assertion */

import * as flatbuffers from 'flatbuffers';

export class OtaInstallProgress {
  bb: flatbuffers.ByteBuffer|null = null;
  bb_pos = 0;
  __init(i:number, bb:flatbuffers.ByteBuffer):OtaInstallProgress {
  this.bb_pos = i;
  this.bb = bb;
  return this;
}

static getRootAsOtaInstallProgress(bb:flatbuffers.ByteBuffer, obj?:OtaInstallProgress):OtaInstallProgress {
  return (obj || new OtaInstallProgress()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

static getSizePrefixedRootAsOtaInstallProgress(bb:flatbuffers.ByteBuffer, obj?:OtaInstallProgress):OtaInstallProgress {
  bb.setPosition(bb.position() + flatbuffers.SIZE_PREFIX_LENGTH);
  return (obj || new OtaInstallProgress()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

task():string|null
task(optionalEncoding:flatbuffers.Encoding):string|Uint8Array|null
task(optionalEncoding?:any):string|Uint8Array|null {
  const offset = this.bb!.__offset(this.bb_pos, 4);
  return offset ? this.bb!.__string(this.bb_pos + offset, optionalEncoding) : null;
}

progress():number {
  const offset = this.bb!.__offset(this.bb_pos, 6);
  return offset ? this.bb!.readFloat32(this.bb_pos + offset) : 0.0;
}

static startOtaInstallProgress(builder:flatbuffers.Builder) {
  builder.startObject(2);
}

static addTask(builder:flatbuffers.Builder, taskOffset:flatbuffers.Offset) {
  builder.addFieldOffset(0, taskOffset, 0);
}

static addProgress(builder:flatbuffers.Builder, progress:number) {
  builder.addFieldFloat32(1, progress, 0.0);
}

static endOtaInstallProgress(builder:flatbuffers.Builder):flatbuffers.Offset {
  const offset = builder.endObject();
  return offset;
}

static createOtaInstallProgress(builder:flatbuffers.Builder, taskOffset:flatbuffers.Offset, progress:number):flatbuffers.Offset {
  OtaInstallProgress.startOtaInstallProgress(builder);
  OtaInstallProgress.addTask(builder, taskOffset);
  OtaInstallProgress.addProgress(builder, progress);
  return OtaInstallProgress.endOtaInstallProgress(builder);
}
}
