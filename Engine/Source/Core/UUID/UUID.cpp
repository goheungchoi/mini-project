#include "UUID.h"

#include <stduuid/uuid.h>

class UUIDRandomGenerator {
  std::random_device rd;
  std::mt19937 generator;
  uuids::uuid_random_generator gen;

 public:
  UUIDRandomGenerator()
      : generator{[this]() {
          std::array<int, std::mt19937::state_size> seed_data{};
          std::generate(std::begin(seed_data), std::end(seed_data),
                        std::ref(rd));
          std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
          return std::mt19937(seq);
        }()},
        gen{generator} {}
  UUIDRandomGenerator(const UUIDRandomGenerator&) = delete;
  UUIDRandomGenerator& operator=(const UUIDRandomGenerator&) = delete;

  uuids::uuid GenerateUUID() { return gen(); }
};

xUUID::xUUID(const uint8_t* data) { memcpy(byte, data, kNumBytes); }

xUUID::xUUID(const std::string& str) {
  auto id = uuids::uuid::from_string(str);
  if (id) {
    std::span sp{id->as_bytes()};
    memcpy(byte, sp.data(), kNumBytes);
  } else {
    memset(byte, '\0', kNumBytes);
  }
}

bool xUUID::IsNil() {
  return uuids::uuid(byte).is_nil();
  
}

std::string xUUID::ToString(bool separated /* = false */) {
  std::span sp(byte);
  std::string s{uuids::to_string(uuids::uuid(sp))};
  if (!separated) {
		auto it = std::remove_if(s.begin(), s.end(), [](char c) { return c == '-'; });
		s.erase(it, s.end());
	}
  return s;
}

bool xUUID::operator==(const xUUID& other) const { return false; }

bool xUUID::IsValidUUIDString(const std::string& uuid) {
  return uuids::uuid::is_valid_uuid(uuid);
}

xUUID GenerateRandomUUID() {
  static UUIDRandomGenerator random_generator{};
  const uuids::uuid id = random_generator.GenerateUUID();
  return xUUID(reinterpret_cast<const uint8_t*>(id.as_bytes().data()));
}

xUUID GenerateUUIDFromName(const std::string& name) {
  static uuids::uuid_name_generator gen(
      uuids::uuid::from_string("47183823-2574-4bfd-b411-99ed177d3e43").value());
  const uuids::uuid id = gen(name);
  return xUUID(reinterpret_cast<const unsigned char*>(id.as_bytes().data()));
}
