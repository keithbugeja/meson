#pragma once

#include <CnsTypes.h>
#include <CnsNamespaces.h>
#include <CnsByteCodeMgr.h>

Meson_Cuneus_BEGIN

class BasicByteCodeMgr : public IByteCodeMgr
{
private:
	byte	*m_pByteCode,
			*m_pOffset;

	size_t	m_unSize;

public:
	BasicByteCodeMgr(void);
	~BasicByteCodeMgr(void);

	virtual void Startup( Meson::Common::Config::Configuration &p_config );
	virtual void Shutdown( void );

	size_t Size( void );
	dword GetOffset( void );
	void AppendByteCode( const byte *p_pbyCode, size_t p_unSize );
	void RemoveByteCode( size_t p_unOffset, size_t p_unSize );
	void GetByteCode( byte *p_pbyCode, size_t p_unOffset, size_t p_unSize );
	byte *GetByteCodePointer( size_t p_unOffset );

protected:
	void ExpandBuffer( void );
};

Meson_Cuneus_END
