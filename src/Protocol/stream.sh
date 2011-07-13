# protoc paths must be absolute
ROOT="/home/aaron/src/DicomStream/src/Protocol"
protoc  --cpp_out=../src/Protocol   --proto_path=$ROOT  $ROOT/stream.proto 
