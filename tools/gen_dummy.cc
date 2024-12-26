// Copyright (c) 2024, Benjamin Shropshire,
// All rights reserved.

#include <fstream>
#include <map>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/log/check.h"
#include "absl/log/initialize.h"
#include "absl/log/log.h"
#include "google/protobuf/descriptor.pb.h"
#include "google/protobuf/dynamic_message.h"
#include "google/protobuf/json/json.h"
#include "json/json.h"

ABSL_FLAG(std::string, counts, "", "");
ABSL_FLAG(std::string, message_name, "", "");
ABSL_FLAG(std::string, pb, "", "");
ABSL_FLAG(std::string, json, "", "");

using google::protobuf::Descriptor;
using google::protobuf::DescriptorPool;
using google::protobuf::DynamicMessageFactory;
using google::protobuf::EnumDescriptor;
using google::protobuf::EnumValueDescriptor;
using google::protobuf::FieldDescriptor;
using google::protobuf::FileDescriptorSet;
using google::protobuf::Message;
using google::protobuf::json::PrintOptions;

namespace {
template <class T>
T Thing(int = 0) {
  static T t = 0;
  return t++;
}

const EnumValueDescriptor *Thing(const EnumDescriptor *ed) {
  static std::map<std::string, int> it;
  int i = it[ed->full_name()]++;
  return ed->value(i % ed->value_count());
}

int Count(const FieldDescriptor *ed) {
  static const auto lens = [] {
    std::map<std::string, int> ret;

    if (auto f = absl::GetFlag(FLAGS_counts); f != "") {
      std::ifstream t(f);
      LOG_IF(QFATAL, t.fail()) << "Failed to open " << f;

      Json::Value root;
      Json::Reader reader;
      CHECK(reader.parse(t, root)) << reader.getFormattedErrorMessages();
      CHECK(root.isObject()) << root;

      for (auto i = root.begin(), e = root.end(); i != e; i++) {
        ret[i.key().asString()] = i->asInt();
      }
    }

    return ret;
  }();

  if (auto i = lens.find(ed->full_name()); i != lens.end()) {
    return i->second;
  }

  LOG(INFO) << ed->full_name();
  return 2;
}

void Populate(Message *pb) {
  const auto *pdd = pb->GetDescriptor();
  const auto *pdr = pb->GetReflection();
  LOG(INFO) << "Populate [" << pdd->full_name() << "]";

  for (int f = 0; f < pdd->field_count(); f++) {
    const auto *fd = pdd->field(f);
    LOG(INFO) << "idx:" << f << " #" << fd->number() << " " << fd->name();

    switch (fd->type()) {
      //////////////////////////////////////////
      case FieldDescriptor::Type::TYPE_GROUP:
        LOG(FATAL) << "Not implemented";

      //////////////////////////////////////////
      case FieldDescriptor::Type::TYPE_FLOAT:
        if (fd->is_repeated()) {
          for (auto i = Count(fd); i--;) {
            pdr->AddFloat(pb, fd, Thing<float>());
          }
        } else {
          pdr->SetFloat(pb, fd, Thing<float>());
        }
        break;

      case FieldDescriptor::Type::TYPE_DOUBLE:
        if (fd->is_repeated()) {
          for (auto i = Count(fd); i--;) {
            pdr->AddDouble(pb, fd, Thing<double>());
          }
        } else {
          pdr->SetDouble(pb, fd, Thing<double>());
        }
        break;

      case FieldDescriptor::Type::TYPE_INT64:
      case FieldDescriptor::Type::TYPE_SINT64:
      case FieldDescriptor::Type::TYPE_SFIXED64:
        if (fd->is_repeated()) {
          for (auto i = Count(fd); i--;) {
            pdr->AddInt64(pb, fd, Thing<std::int64_t>());
          }
        } else {
          pdr->SetInt64(pb, fd, Thing<std::int64_t>());
        }
        break;

      case FieldDescriptor::Type::TYPE_UINT64:
      case FieldDescriptor::Type::TYPE_FIXED64:
        if (fd->is_repeated()) {
          for (auto i = Count(fd); i--;) {
            pdr->AddUInt64(pb, fd, Thing<std::uint64_t>());
          }
        } else {
          pdr->SetUInt64(pb, fd, Thing<std::uint64_t>());
        }
        break;

      case FieldDescriptor::Type::TYPE_INT32:
      case FieldDescriptor::Type::TYPE_SINT32:
      case FieldDescriptor::Type::TYPE_SFIXED32:
        if (fd->is_repeated()) {
          for (auto i = Count(fd); i--;) {
            pdr->AddInt32(pb, fd, Thing<std::int32_t>());
          }
        } else {
          pdr->SetInt32(pb, fd, Thing<std::int32_t>());
        }
        break;

      case FieldDescriptor::Type::TYPE_UINT32:
      case FieldDescriptor::Type::TYPE_FIXED32:
        if (fd->is_repeated()) {
          for (auto i = Count(fd); i--;) {
            pdr->AddUInt32(pb, fd, Thing<std::uint32_t>());
          }
        } else {
          pdr->SetUInt32(pb, fd, Thing<std::uint32_t>());
        }
        break;

        //////////////////////////////////////////

      case FieldDescriptor::Type::TYPE_BOOL:
        if (fd->is_repeated()) {
          for (auto i = Count(fd); i--;) {
            pdr->AddBool(pb, fd, true);
          }
        } else {
          pdr->SetBool(pb, fd, true);
        }
        break;

      case FieldDescriptor::Type::TYPE_STRING:
      case FieldDescriptor::Type::TYPE_BYTES:
        if (fd->is_repeated()) {
          for (auto i = Count(fd); i--;) {
            pdr->AddString(pb, fd, absl::StrCat("S-", int{Thing<char>()}));
          }
        } else {
          pdr->SetString(pb, fd, absl::StrCat("S-", int{Thing<char>()}));
        }
        break;

      case FieldDescriptor::Type::TYPE_MESSAGE:
        if (fd->is_repeated()) {
          for (auto i = Count(fd); i--;) {
            Populate(pdr->AddMessage(pb, fd));
          }
        } else {
          Populate(pdr->MutableMessage(pb, fd));
        }
        break;

      case FieldDescriptor::Type::TYPE_ENUM:
        if (fd->is_repeated()) {
          for (auto i = Count(fd); i--;) {
            pdr->AddEnum(pb, fd, Thing(fd->enum_type()));
          }
        } else {
          pdr->SetEnum(pb, fd, Thing(fd->enum_type()));
        }
        break;
    }
  }
}

}  // namespace

int main(int argc, char **argv) {
  auto args = absl::ParseCommandLine(argc, argv);
  absl::InitializeLog();

  LOG_IF(QFATAL, absl::GetFlag(FLAGS_message_name) == "")
      << "--message_name is required";

  args.erase(args.begin());  // Remove argv0 (the binary)
  LOG_IF(QFATAL, args.size() == 0) << "must provide sources.";

  DescriptorPool pool;
  for (const std::string f : args) {
    LOG(INFO) << "Proccessing " << f;
    std::ifstream t(f);
    LOG_IF(QFATAL, t.fail()) << "Failed to open " << f;
    std::stringstream b;
    b << t.rdbuf();
    std::string data = b.str();

    FileDescriptorSet file;
    if (!file.ParseFromString(data)) {
      LOG(QFATAL) << f << " is not a FileDescriptorSet\n";
      return 1;
    }
    // LOG(INFO) << file.IsInitialized() << " -> " << file.DebugString();

    if (file.file_size() == 0) {
      LOG(QFATAL) << f << " contains " << file.file_size()
                  << " files. Expected at least 1.\n";
      return 1;
    }

    for (const auto &file : file.file()) pool.BuildFile(file);
  }

  auto *desc = pool.FindMessageTypeByName(absl::GetFlag(FLAGS_message_name));
  LOG_IF(QFATAL, !desc) << "Dind't find pb: "
                        << absl::GetFlag(FLAGS_message_name);

  DynamicMessageFactory fact(&pool);
  auto pb = absl::WrapUnique(fact.GetPrototype(desc)->New());

  Populate(pb.get());

  LOG(INFO) << pb->IsInitialized();

  if (absl::GetFlag(FLAGS_pb) != "") {
    std::ofstream pb_out(absl::GetFlag(FLAGS_pb));
    LOG_IF(QFATAL, !pb_out) << "Failed to open " << absl::GetFlag(FLAGS_pb);

    pb_out << pb->DebugString();
  }

  if (absl::GetFlag(FLAGS_json) != "") {
    // Render the JSON object as a string.
    PrintOptions opt;
    opt.preserve_proto_field_names = true;
    opt.always_print_enums_as_ints = true;
    opt.add_whitespace = true;

    std::string json;
    auto ret_status = MessageToJsonString(*pb, &json, opt);
    LOG(INFO) << ret_status;

    std::ofstream json_out(absl::GetFlag(FLAGS_json));
    LOG_IF(QFATAL, !json_out) << "Failed to open " << absl::GetFlag(FLAGS_json);

    json_out << json;
  }

  return 0;
}
