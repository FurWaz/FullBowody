# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: mediapipe/calculators/util/packet_frequency_calculator.proto

import sys
_b=sys.version_info[0]<3 and (lambda x:x) or (lambda x:x.encode('latin1'))
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
from google.protobuf import descriptor_pb2
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()


from mediapipe.framework import calculator_pb2 as mediapipe_dot_framework_dot_calculator__pb2
mediapipe_dot_framework_dot_calculator__options__pb2 = mediapipe_dot_framework_dot_calculator__pb2.mediapipe_dot_framework_dot_calculator__options__pb2


DESCRIPTOR = _descriptor.FileDescriptor(
  name='mediapipe/calculators/util/packet_frequency_calculator.proto',
  package='mediapipe',
  syntax='proto2',
  serialized_pb=_b('\n<mediapipe/calculators/util/packet_frequency_calculator.proto\x12\tmediapipe\x1a$mediapipe/framework/calculator.proto\"\xa8\x01\n PacketFrequencyCalculatorOptions\x12\x1a\n\x0ftime_window_sec\x18\x01 \x01(\x01:\x01\x33\x12\r\n\x05label\x18\x02 \x03(\t2Y\n\x03\x65xt\x12\x1c.mediapipe.CalculatorOptions\x18\xb6\xc3\xaaP \x01(\x0b\x32+.mediapipe.PacketFrequencyCalculatorOptions')
  ,
  dependencies=[mediapipe_dot_framework_dot_calculator__pb2.DESCRIPTOR,])
_sym_db.RegisterFileDescriptor(DESCRIPTOR)




_PACKETFREQUENCYCALCULATOROPTIONS = _descriptor.Descriptor(
  name='PacketFrequencyCalculatorOptions',
  full_name='mediapipe.PacketFrequencyCalculatorOptions',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='time_window_sec', full_name='mediapipe.PacketFrequencyCalculatorOptions.time_window_sec', index=0,
      number=1, type=1, cpp_type=5, label=1,
      has_default_value=True, default_value=float(3),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='label', full_name='mediapipe.PacketFrequencyCalculatorOptions.label', index=1,
      number=2, type=9, cpp_type=9, label=3,
      has_default_value=False, default_value=[],
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
    _descriptor.FieldDescriptor(
      name='ext', full_name='mediapipe.PacketFrequencyCalculatorOptions.ext', index=0,
      number=168468918, type=11, cpp_type=10, label=1,
      has_default_value=False, default_value=None,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=True, extension_scope=None,
      options=None),
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
  serialized_start=114,
  serialized_end=282,
)

DESCRIPTOR.message_types_by_name['PacketFrequencyCalculatorOptions'] = _PACKETFREQUENCYCALCULATOROPTIONS

PacketFrequencyCalculatorOptions = _reflection.GeneratedProtocolMessageType('PacketFrequencyCalculatorOptions', (_message.Message,), dict(
  DESCRIPTOR = _PACKETFREQUENCYCALCULATOROPTIONS,
  __module__ = 'mediapipe.calculators.util.packet_frequency_calculator_pb2'
  # @@protoc_insertion_point(class_scope:mediapipe.PacketFrequencyCalculatorOptions)
  ))
_sym_db.RegisterMessage(PacketFrequencyCalculatorOptions)

_PACKETFREQUENCYCALCULATOROPTIONS.extensions_by_name['ext'].message_type = _PACKETFREQUENCYCALCULATOROPTIONS
mediapipe_dot_framework_dot_calculator__options__pb2.CalculatorOptions.RegisterExtension(_PACKETFREQUENCYCALCULATOROPTIONS.extensions_by_name['ext'])

# @@protoc_insertion_point(module_scope)
