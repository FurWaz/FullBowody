# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: mediapipe/calculators/util/packet_frequency.proto

import sys
_b=sys.version_info[0]<3 and (lambda x:x) or (lambda x:x.encode('latin1'))
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
from google.protobuf import descriptor_pb2
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor.FileDescriptor(
  name='mediapipe/calculators/util/packet_frequency.proto',
  package='mediapipe',
  syntax='proto2',
  serialized_pb=_b('\n1mediapipe/calculators/util/packet_frequency.proto\x12\tmediapipe\"=\n\x0fPacketFrequency\x12\x1b\n\x13packet_frequency_hz\x18\x01 \x01(\x01\x12\r\n\x05label\x18\x02 \x01(\t')
)
_sym_db.RegisterFileDescriptor(DESCRIPTOR)




_PACKETFREQUENCY = _descriptor.Descriptor(
  name='PacketFrequency',
  full_name='mediapipe.PacketFrequency',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='packet_frequency_hz', full_name='mediapipe.PacketFrequency.packet_frequency_hz', index=0,
      number=1, type=1, cpp_type=5, label=1,
      has_default_value=False, default_value=float(0),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='label', full_name='mediapipe.PacketFrequency.label', index=1,
      number=2, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=_b("").decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  syntax='proto2',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=64,
  serialized_end=125,
)

DESCRIPTOR.message_types_by_name['PacketFrequency'] = _PACKETFREQUENCY

PacketFrequency = _reflection.GeneratedProtocolMessageType('PacketFrequency', (_message.Message,), dict(
  DESCRIPTOR = _PACKETFREQUENCY,
  __module__ = 'mediapipe.calculators.util.packet_frequency_pb2'
  # @@protoc_insertion_point(class_scope:mediapipe.PacketFrequency)
  ))
_sym_db.RegisterMessage(PacketFrequency)


# @@protoc_insertion_point(module_scope)
