/*
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above
 *   copyright notice, this list of conditions and the following disclaimer
 *   in the documentation and/or other materials provided with the
 *   distribution.
 * * Neither the name of the  nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

/**
 * @file
 * @author Ricardo Román <reroman4@gmail.com>
 * @brief Declaración de la clase reroman::IPv4Addr.
 */

#ifndef REROMAN_IPV4ADDR_HPP
#define REROMAN_IPV4ADDR_HPP

#include <iostream>
#include <string>
#include <stdexcept>
#include <system_error>

#include <cstdint>

#include <netinet/in.h>
#include <arpa/inet.h>

namespace reroman{ class IPv4Addr; }

/**
 * @brief Coloca una dirección IPv4 en un flujo de salida.
 * @param out Flujo de salida en el cual se insertará la IP.
 * @param ip Dirección que se insertará en el flujo de salida.
 * @return Una referencia a out.
 */
std::ostream& operator<<( std::ostream &out, const reroman::IPv4Addr &ip );

/**
 * @brief Versión conmutativa de IPv4Addr + int.
 * @details Funciona de la misma manera que
 * reroman::IPv4Addr::operator+(int n) const
 */
reroman::IPv4Addr operator+( int n, const reroman::IPv4Addr &ip );


namespace reroman
{
	/**
	 * @brief Representación de una dirección IPv4.
	 * @headerfile ipv4addr.hpp <reroman/ipv4addr.hpp>
	 */
	class IPv4Addr
	{
		friend std::ostream& ::operator<<( std::ostream &out, const IPv4Addr &ip );

	public:
		//===============================================================
		//							Constructores
		//===============================================================
		/**
		 * @brief Inicializa una dirección IP a partir de un entero de 4 bytes en
		 * formato de red.
		 * @param addr Entero de 4 bytes en formato de red que contiene el valor de la IP.
		 */
		explicit IPv4Addr( uint32_t addr = 0 ) noexcept;

		/**
		 * @brief Obtiene la dirección IP a partir de una cadena con notación de números
		 * y puntos.
		 * @details Internamente se utiliza la función inet_aton() por lo que aplica los
		 * mismos formatos para addr.
		 * @param addr Cadena en notación de números y puntos que representan la dirección
		 * IP.
		 * @throw std::invalid_argument si la cadena no es una dirección IP válida.
		 */
		IPv4Addr( std::string addr );

		/**
		 * @brief Crea una dirección IP a partir de una estructura in_addr
		 * @param addr Estructura que contiene la información de la IP.
		 */
		IPv4Addr( struct in_addr addr ) noexcept;

		/**
		 * @brief Crea una nueva dirección IP a partir de otra ya existente.
		 * @param addr Objeto del cual crear la copia.
		 */
		IPv4Addr( const IPv4Addr &addr ) noexcept;


		//===============================================================
		//							Getters
		//===============================================================
		/**
		 * @brief Obtiene la representación en cadena de una IP.
		 * @return Una cadena en formato \b x.x.x.x.
		 */
		std::string toString( void ) const;

		/**
		 * @brief Obtiene la estructura in_addr de la dirección IP.
		 * @return La estructura in_addr equivalente.
		 */
		const struct in_addr& getInAddr( void ) const noexcept;

		/**
		 * @brief Obtiene un entero representativo de la IP.
		 * @return Un entero de 4 bytes en formato de red.
		 */
		uint32_t toNetworkInt( void ) const noexcept;

		/**
		 * @brief Obtiene un entero representativo de la IP.
		 * @return Un entero de 4 bytes en formato de host.
		 */
		uint32_t toHostInt( void ) const noexcept;

		/**
		 * @brief Verifica si es la dirección 0.0.0.0.
		 * @return Verdadero si todos los bytes son 0, falso en caso
		 * contrario.
		 */
		bool isNull( void ) const noexcept;

		/**
		 * @brief Establece la dirección 0.0.0.0.
		 */
		void clear( void ) noexcept;

		/**
		 * @brief Verifica si la IP contiene un valor válido para una máscara
		 * de subred.
		 * @return Verdadero si es un valor válido, falso en caso contrario.
		 */
		bool isValidNetmask( void ) const noexcept;


		//===============================================================
		//							Setters
		//===============================================================
		/**
		 * @brief Establece una nueva dirección IP a partir de una cadena
		 * en notación de puntos y números.
		 * @details La función utiliza inet_aton para la conversión.
		 * @param addr Cadena en notación de puntos y números.
		 * @throw std::invalid_argument si la dirección no es válida.
		 */
		void setAddr( std::string addr );

		/**
		 * @brief Establece una nueva dirección IP a partir de un entero
		 * de 4 bytes en formato de red.
		 * @param addr Entero en formato de red.
		 */
		void setAddr( uint32_t addr ) noexcept;

		/**
		 * @brief Establece una nueva dirección IP a partir de una estructura
		 * in_addr.
		 * @param addr Estructura que contiene la nueva dirección.
		 */
		void setAddr( struct in_addr addr ) noexcept;


		//===============================================================
		//							Operadores
		//===============================================================
		/**
		 * @brief Copia los datos de una dirección IP a otra.
		 * @param addr Objeto del cual se van a copiar los datos.
		 * @return Una referencia a *this.
		 */
		IPv4Addr& operator=( const IPv4Addr &addr ) noexcept = default;

		/**
		 * @brief Verifica si dos direcciones IP son iguales.
		 * @param addr Dirección con la que se desea comparar.
		 * @return Verdadero si la dirección contiene el mismo valor, falso
		 * en caso contrario.
		 */
		bool operator==( const IPv4Addr &addr ) const noexcept;

		/**
		 * @brief Compara si dos direcciones IP son diferentes.
		 * @param addr Dirección con la que se desea comparar.
		 * @return Verdadero si la dirección contiene un valor diferente,
		 * falso en caso contrario.
		 */
		bool operator!=( const IPv4Addr &addr ) const noexcept;

		/**
		 * @brief Compara si la dirección es menor que otra.
		 * @param addr Dirección con la que se desea comparar.
		 * @return Verdadero si la dirección es menor que addr,
		 * falso en caso contrario.
		 */
		bool operator<( const IPv4Addr &addr ) const noexcept;

		/**
		 * @brief Suma un entero a una dirección IP.
		 * @param n El número de hosts a sumar a la dirección.
		 * @return Una IP resultado de sumar n hosts a la dirección original.
		 * @throw std::overflow_error si el resultado sobrepasa la IP
		 * 255.255.255.255.
		 * @throw std::underflow_error si n es negativo y el resultado
		 * es menor que 0.0.0.0.
		 * @see operator+( int n, const reroman::IPv4Addr &ip )
		 */
		IPv4Addr operator+( int n ) const;

		/**
		 * @brief Resta un entero a una dirección IP.
		 * @param n El número de hosts a restar a la dirección.
		 * @return Una IP resultado de restar n hosts a la dirección original.
		 * @throw std::overflow_error si n es negativo y el resultado es mayor que
		 * 255.255.255.255.
		 * @throw std::underflow_error si el resultado es menor a la IP 0.0.0.0.
		 */
		IPv4Addr operator-( int n ) const;

		/**
		 * @brief Preincrementa en uno la dirección IP.
		 * @return El valor de *this ya incrementado.
		 * @throw std::overflow_error si el valor resultante es mayor que
		 * 255.255.255.255. El objeto permanece sin cambios.
		 */
		IPv4Addr& operator++( void );

		/**
		 * @brief Postincrementa en uno la dirección IP.
		 * @return El valor de *this antes del incremento.
		 * @throw std::overflow_error si el valor resultante es mayor que
		 * 255.255.255.255. El objeto permanece sin cambios.
		 */
		IPv4Addr operator++( int );

		/**
		 * @brief Predecrementa en uno la dirección IP.
		 * @return El valor de *this ya incrementado.
		 * @throw std::underflow_error si el valor resultante es menor que
		 * 0.0.0.0. El objeto permanece sin cambios.
		 */
		IPv4Addr& operator--( void );

		/**
		 * @brief Postdecrementa en uno la dirección IP.
		 * @return El valor de *this antes del incremento.
		 * @throw std::underflow_error si el valor resultante es menor que
		 * 0.0.0.0. El objeto permanece sin cambios.
		 */
		IPv4Addr operator--( int );

		/**
		 * @brief Efectúa una operación NOT a nivel de bit.
		 * @return Una dirección IP resultado de la operación NOT.
		 */
		IPv4Addr operator~( void ) const noexcept;

		/**
		 * @brief Efectúa una operación AND bit a bit entre direcciones IP.
		 * @param addr Dirección con la cual efectuar la operación.
		 * @return Una dirección IP resultado de la operación AND.
		 */
		IPv4Addr operator&( const IPv4Addr &addr ) const noexcept;

		/**
		 * @brief Efectúa una operación OR bit a bit entre direcciones IP.
		 * @param addr Dirección con la cual efectuar la operación.
		 * @return Una dirección IP resultado de la operación OR.
		 */
		IPv4Addr operator|( const IPv4Addr &addr ) const noexcept;

		/**
		 * @brief Efectúa una operación XOR bit a bit entre direcciones IP.
		 * @param addr Dirección con la cual efectuar la operación.
		 * @return Una dirección IP resultado de la operación XOR.
		 */
		IPv4Addr operator^( const IPv4Addr &addr ) const noexcept;


		//===============================================================
		//						Miembros Estáticos
		//===============================================================
		static constexpr int IPv4AddrLen = 4; ///< Longitud en bytes de una dirección IPv4

		/**
		 * @brief Obtiene la dirección IPv4 de una interfaz de red.
		 * @param ifname Nombre de la interfaz de red.
		 * @return La dirección IPv4 de la interfaz de red especificada.
		 * @throw std::system_error si ocurre algún error.
		 */
		static IPv4Addr getFromInterface( std::string ifname );

		/**
		 * @brief Obtiene la máscara de subred de una interfaz de red.
		 * @param ifname Nombre de la interfaz de red.
		 * @return La máscara de red de la interfaz especificada.
		 * @throw std::system_error si ocurre algún error.
		 */
		static IPv4Addr getNmaskFromInterface( std::string ifname );

		/**
		 * @brief Obtiene la dirección IP de red.
		 * @param host Dirección IP de cualquier host en la red.
		 * @param netmask Máscara de subred del host.
		 * @return La dirección IP a la cual pertenece el host.
		 * @note La función no valida la máscara de subred. El usuario puede
		 * asegurarse de que es una máscara válida con isValidNetmask() const.
		 */
		static IPv4Addr makeNetAddress( const IPv4Addr &host, const IPv4Addr &netmask )
			noexcept;

		/**
		 * @brief Obtiene la dirección IP de broadcast.
		 * @param host Dirección IP de cualquier host en la red.
		 * @param netmask Máscara de subred del host.
		 * @return La dirección de broadcast en la red a la cual pertenece el host.
		 * @note La función no valida la máscara de subred. El usuario puede
		 * asegurarse de que es una máscara válida con isValidNetmask() const.
		 */
		static IPv4Addr makeBroadcast( const IPv4Addr &host, const IPv4Addr &netmask )
			noexcept;

	private:
		struct in_addr data;
	};

	//===============================================================
	//					Métodos Inline	
	//===============================================================
	inline const struct in_addr& IPv4Addr::getInAddr( void ) const noexcept
	{
		return data;
	}

	inline uint32_t IPv4Addr::toNetworkInt( void ) const noexcept
	{
		return data.s_addr;
	}

	inline uint32_t IPv4Addr::toHostInt( void ) const noexcept
	{
		return ntohl( data.s_addr );
	}

	inline bool IPv4Addr::isNull( void ) const noexcept
	{
		return !data.s_addr;
	}

	inline void IPv4Addr::clear( void ) noexcept
	{
		data.s_addr = 0;
	}

	inline void IPv4Addr::setAddr( uint32_t addr ) noexcept
	{
		data.s_addr = addr;
	}

	inline void IPv4Addr::setAddr( struct in_addr addr ) noexcept
	{
		data = addr;
	}

	inline bool IPv4Addr::operator==( const IPv4Addr &addr )
		const noexcept
	{
		return data.s_addr == addr.data.s_addr;
	}

	inline bool IPv4Addr::operator!=( const IPv4Addr &addr )
		const noexcept
	{
		return data.s_addr != addr.data.s_addr;
	}

	inline bool IPv4Addr::operator<( const IPv4Addr &addr )
		const noexcept
	{
		return data.s_addr < addr.data.s_addr;
	}

	inline IPv4Addr IPv4Addr::operator~( void ) const noexcept
	{
		return IPv4Addr( ~data.s_addr );
	}

	inline IPv4Addr IPv4Addr::operator&( const IPv4Addr &addr )
		const noexcept
	{
		return IPv4Addr( data.s_addr & addr.data.s_addr );
	}

	inline IPv4Addr IPv4Addr::operator|( const IPv4Addr &addr )
		const noexcept
	{
		return IPv4Addr( data.s_addr | addr.data.s_addr );
	}

	inline IPv4Addr IPv4Addr::operator^( const IPv4Addr &addr )
		const noexcept
	{
		return IPv4Addr( data.s_addr ^ addr.data.s_addr );
	}

	inline IPv4Addr IPv4Addr::makeNetAddress( const IPv4Addr &host,
		const IPv4Addr &netmask ) noexcept
	{
		return host & netmask;
	}

	inline IPv4Addr IPv4Addr::makeBroadcast( const IPv4Addr &host,
		const IPv4Addr &netmask ) noexcept
	{
		return host | ~netmask;
	}
}// namespace reroman

inline std::ostream& operator<<( std::ostream &out, const
		reroman::IPv4Addr &ip )
{
	out << ip.toString();
	return out;
}

inline reroman::IPv4Addr operator+( int n, const reroman::IPv4Addr &ip )
{
	return ip + n;
}

#endif // REROMAN_IPV4ADDR_HPP

