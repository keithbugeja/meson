#pragma once

#include "Namespaces.h"
#include "Pointer.h"

Meson_Common_IO_BEGIN

class IStream
	: public Meson::Common::TReferenceCounter<IStream>
{
public:
	IStream(void) {};
	virtual ~IStream(void) {};
	virtual bool IsOpen(void) const = 0;
	virtual bool IsEndOfStream(void) const = 0;
	virtual void Reopen(void) = 0;
	virtual void Close(void) = 0;
};

typedef Meson::Common::TPointer<IStream> StreamPtr;

Meson_Common_IO_END
