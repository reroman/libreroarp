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
 * @brief Declaración de la clase reroman::HwAddr.
 */

#ifndef REROMAN_HWADDR_HPP
#define REROMAN_HWADDR_HPP

#include <iostream>
#include <string>
#include <stdexcept>
#include <system_error>
#include <initializer_list>
#include <array>

#include <cstdint>

#include <net/ethernet.h>

namespace reroman{ class HwAddr; }

/**
 * @brief Coloca una dirección física en un flujo de salida.
 * @details La dirección es insertada en flujo de salida, además
 * del vendedor entre paréntesis.
 * @param out Flujo de salida en el cual se insertará la dirección física.
 * @param addr Dirección que se insertará en el flujo de salida.
 * @return Una referencia a out.
 */
std::ostream& operator<<( std::ostream &out, const reroman::HwAddr &addr );

/**
 * @brief Espacio de nombres propio del autor.
 */
namespace reroman
{
	/**
	 * @brief Representación de una dirección física.
	 * @headerfile hwaddr.hpp <reroman/hwaddr.hpp>
	 */
	class HwAddr final
	{
		friend std::ostream& ::operator<<( std::ostream &out, const HwAddr &addr );

	public:
		//===============================================================
		//							Constructores
		//===============================================================
		/**
		 * @brief Inicializa una dirección física estableciendo todos sus bytes a 0.
		 */
		HwAddr( void ) noexcept;

		/**
		 * @brief Inicializa una dirección física a partir de una cadena en formato
		 * \b xx:xx:xx:xx:xx:xx.
		 * @param addr Cadena que contiene la dirección física.
		 * @throw std::invalid_argument si la cadena no es una dirección física válida y en
		 * el formato establecido.
		 */
		HwAddr( std::string addr  );

		/**
		 * @brief Inicializa una dirección física a partir de una lista de valores.
		 * @details Pasar como argumento { 1, 2, 3, 4, 5, 6 } generará la dirección
		 * 01:02:03:04:05:06.
		 * @param bytes Lista de bytes con los valores a inicializar.
		 * @throw std::invalid_argument si la lista de bytes no es de al menos
		 * HwAddrLen elementos.
		 */
		HwAddr( std::initializer_list<uint8_t> bytes );

		/**
		 * @brief Inicializa una dirección física a partir de una estructura ether_addr.
		 * @param addr Apuntador a la estructura que contiene los valores de la dirección.
		 */
		HwAddr( const struct ether_addr *addr );

		/**
		 * @brief Inicializa una dirección física a partir de un arreglo estilo C.
		 * @details Se tomarán los primeros HwAddrLen bytes del arreglo.
		 * @param bytes Apuntador a la primer posición del arreglo.
		 */
		HwAddr( const uint8_t *bytes );

		/**
		 * @brief Constructor de copia
		 * @param addr Dirección física de la cual obtener los datos.
		 */
		HwAddr( const HwAddr &addr ) noexcept;


		//===============================================================
		//							Getters
		//===============================================================
		/**
		 * @brief Verifica si todos los bytes de la dirección son 0.
		 * @return Verdadero si todos los bytes son 0, falso en caso contrario.
		 */
		bool isNull( void ) const noexcept;

		/**
		 * @brief Obtiene la representación en cadena de una dirección física.
		 * @return La representación del objeto en forma de cadena en formato
		 * \b xx:xx:xx:xx:xx:xx.
		 */
		std::string toString( void ) const;

		/**
		 * @brief Obtiene el arreglo en el cual se almacenan los datos del objeto.
		 * @warning No se recomienda alterar los valores directamente, sino utilizar
		 * HwAddr::setByte(), ya que puede dejar al objeto en un estado inconsistente.
		 * @return Un apuntador a los datos del objeto.
		 */
		const uint8_t* getData( void ) const noexcept;

		/**
		 * @brief Obtiene el valor de un byte de la dirección física.
		 * @param index El índice del byte del cual se quiere obtener el valor.
		 * @return El valor del byte.
		 * @throw std::out_of_range si index se encuentra fuera del rango
		 * [0,HwAddrLen].
		 */
		uint8_t getByte( unsigned int index ) const;

		/**
		 * @brief Obtiene el nombre del vendedor del dispositivo.
		 * @details La consulta se hace desde el archivo vendors.db. Si
		 * el archivo o la dirección física no es encontrada la función devuelve
		 * "Unknown". Se debe haber compilado con la opción ENABLE_VENDORS.
		 * @return Una cadena con el nombre del vendedor.
		 */
		std::string getVendor( void ) const;


		//===============================================================
		//							Setters
		//===============================================================
		/**
		 * @brief Establece una nueva dirección a partir de una cadena.
		 * @param addr Dirección en formato \b xx:xx:xx:xx:xx:xx.
		 * @throw std::invalid_argument si la cadena no es una dirección válida.
		 */
		void setData( std::string addr );

		/**
		 * @brief Establece una dirección física a partir de una lista de valores
		 * para los bytes.
		 * @param bytes Lista de valores para los bytes de la nueva dirección física.
		 * @throw std::invalid_argument si la lista de bytes no es de al menos
		 * HwAddrLen elementos.
		 */
		void setData( std::initializer_list<unsigned char> bytes );

		/**
		 * @brief Establece una nueva dirección física a partir de una estructura
		 * ether_addr.
		 * @param addr Apuntador a una estructura ether_addr.
		 */
		void setData( const struct ether_addr *addr );

		/**
		 * @brief Establece una nueva dirección física a partir de un arreglo estilo
		 * C, utilizando los 6 primeros bytes.
		 * @param bytes Apuntador al primer byte del arreglo.
		 */
		void setData( const uint8_t *bytes );

		/**
		 * @brief Establece el valor de un byte en la dirección física.
		 * @param index Índice a cambiar en la dirección.
		 * @param value Nuevo valor para el byte.
		 * @throw std::out_of_range si index es mayor a HwAddrLen.
		 */
		void setByte( unsigned int index, uint8_t value );

		/**
		 * @brief Establece todos los bytes de la dirección a 0.
		 */
		void clear( void ) noexcept;


		//===============================================================
		//							Operadores
		//===============================================================
		/**
		 * @brief Copia los datos de una dirección física.
		 * @param addr Objeto del cual se van a copiar los datos.
		 * @return Una referencia al objeto.
		 */
		HwAddr& operator =( const HwAddr &addr ) noexcept = default;

		/**
		 * @brief Compara dos direcciones físicas.
		 * @param addr Dirección con la cual comparar el objeto.
		 * @return Verdadero si todos los bytes coinciden entre los objetos;
		 * falso en caso contrario.
		 */
		bool operator ==( const HwAddr &addr ) const noexcept;

		/**
		 * @brief Verifica si dos direcciones físicas son diferentes.
		 * @param addr Dirección con la cual se compara el objeto.
		 * @return Verdadero si las direcciones son diferentes; falso si
		 * son iguales.
		 */
		bool operator !=( const HwAddr &addr ) const noexcept;


		//===============================================================
		//						Miembros Estáticos
		//===============================================================
		static constexpr int HwAddrLen = 6; ///< Establece la longitud en bytes de una dirección física.

		/**
		 * @brief Obtiene la dirección física de una interfaz de red.
		 * @param ifname Nombre de la interfaz de red de la cual obtener
		 * los datos.
		 * @return La dirección física de la interfaz de red especificada.
		 * @throw std::system_error si existe algún error con la interfaz de red.
		 */
		static HwAddr getFromInterface( std::string ifname );

	private:
		std::array<uint8_t, HwAddrLen> data;
	};


	//===============================================================
	//					Métodos Inline	
	//===============================================================
	inline const uint8_t* HwAddr::getData() const noexcept
	{
		return data.data();
	}

	inline uint8_t HwAddr::getByte( unsigned int index ) const
	{
		return data.at( index );
	}

	inline void HwAddr::setData( const struct ether_addr *addr )
	{
		for( int i = 0 ; i < HwAddrLen ; i++ )
			data[i] = addr->ether_addr_octet[i];
	}

	inline void HwAddr::setData( const uint8_t *bytes )
	{
		for( int i = 0 ; i < HwAddrLen ; i++ )
			data[i] = bytes[i];
	}

	inline void HwAddr::setByte( unsigned int index, uint8_t value )
	{
		data.at( index ) = value;
	}

	inline void HwAddr::clear() noexcept
	{
		data.fill( 0 );
	}

	inline bool HwAddr::operator ==( const HwAddr &addr ) const noexcept
	{
		return data == data;
	}

	inline bool HwAddr::operator !=( const HwAddr &addr ) const noexcept
	{
		return data != addr.data;
	}
} // namespace reroman

inline std::ostream& operator <<( std::ostream &out, const reroman::HwAddr &addr )
{
	out << addr.toString() << " (" << addr.getVendor() << ')';
	return out;
}

#endif // REROMAN_HWADDR_HPP
