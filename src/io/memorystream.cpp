#include "memorystream.h"

#include <algorithm>

namespace Impacto {
namespace Io {

MemoryStream::MemoryStream(void const* mem, int64_t size, bool freeOnClose)
    : Memory(mem), FreeOnClose(freeOnClose) {
  Meta.Size = size;
  IsMemory = true;
}

int64_t MemoryStream::Read(void* buffer, int64_t sz) {
  if (sz < 0) return IoError_Fail;
  if (Position == Meta.Size) return IoError_Eof;
  sz = std::min(Meta.Size - Position, sz);
  memcpy(buffer, (uint8_t*)Memory + Position, sz);
  Position += sz;
  return sz;
}

int64_t MemoryStream::Seek(int64_t offset, int origin) {
  int64_t newPos = Position;
  if (origin == RW_SEEK_SET) {
    newPos = offset;
  } else if (origin == RW_SEEK_CUR) {
    newPos += offset;
  } else if (origin = RW_SEEK_END) {
    newPos = Meta.Size - offset;
  } else {
    return IoError_Fail;
  }
  if (newPos < 0 || newPos > Meta.Size) return IoError_Fail;
  Position = newPos;
  return newPos;
}

IoError MemoryStream::Duplicate(InputStream** outStream) {
  MemoryStream* result = new MemoryStream(*this);
  result->FreeOnClose = false;
  *outStream = (InputStream*)result;
  return IoError_OK;
}

}  // namespace Io
}  // namespace Impacto