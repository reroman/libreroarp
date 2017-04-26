/**
 * @file
 * @author Ricardo Román <reroman4@gmail.com>
 * @brief Declaración de la clase reroman::NetworkInterface.
 */

#ifndef REROMAN_NETWORKINTERFACE_HPP
#define REROMAN_NETWORKINTERFACE_HPP

#include <reroman/ipv4addr.hpp>
#include <reroman/hwaddr.hpp>

namespace reroman
{
	/**
	 * @brief Representación de una interfaz de red en el sistema.
	 * @headerfile networkinterface.hpp <reroman/networkinterface.hpp>
	 */
	class NetworkInterface
	{
	public:
		//===============================================================
		//							Constructores
		//===============================================================
		/**
		 * @brief Obtiene los datos de una interfaz de red en el sistema
		 * a partir de su nombre.
		 * @param ifname Nombre de la interfaz de red.
		 * @throw std::system_error si el nombre de la interfaz no existe.
		 */
		explicit NetworkInterface( std::string ifname )
			throw( std::system_error );

		/**
		 * @brief Obtiene los datos de una interfaz de red en el sistema
		 * a partir de su índice.
		 * @param index Índice de la interfaz.
		 * @throw std::system_error si el índice de la interfaz no existe.
		 */
		explicit NetworkInterface( int index )
			throw( std::system_error );


		//===============================================================
		//							Getters
		//===============================================================
		/**
		 * @brief Obtiene el nombre de la interfaz de red.
		 * @return Una cadena con el nombre de la interfaz.
		 */
		std::string getName( void ) const noexcept;

		/**
		 * @brief Obtiene el índice de la interfaz.
		 * @return Un entero, índice de la interfaz.
		 */
		int getIndex( void ) const noexcept;

		/**
		 * @brief Obtiene la dirección IPv4 asignada a la interfaz de red.
		 * @return Un objeto IPv4Addr con la dirección.
		 * @throw std::system_error si no se puede obtener la dirección
		 * requerida.
		 */
		IPv4Addr getAddress( void )
			throw( std::system_error );

		/**
		 * @brief Obtiene la máscara de subred asignada a la interfaz.
		 * @return Un objeto IPv4Addr con la máscara de subred.
		 * @throw std::system_error si no se puede obtener la dirección
		 * requerida.
		 */
		IPv4Addr getNetmask( void )
			throw( std::system_error );

		/**
		 * @brief Obtiene la dirección física asignada a la interfaz.
		 * @return Un objeto HwAddr con la dirección de hardware del dispositivo.
		 * @throw std::system_error si no se puede obtener la dirección
		 * requerida.
		 */
		HwAddr getHwAddress( void )
			throw( std::system_error );

	private:
		std::string name;
		int index;
	};


	//===============================================================
	//					Métodos Inline	
	//===============================================================
	inline std::string NetworkInterface::getName( void ) const noexcept
	{
		return name;
	}

	inline int NetworkInterface::getIndex( void ) const noexcept
	{
		return index;
	}

	inline IPv4Addr NetworkInterface::getAddress( void )
		throw( std::system_error )
	{
		return IPv4Addr::getFromInterface( name );
	}

	inline IPv4Addr NetworkInterface::getNetmask( void )
		throw( std::system_error )
	{
		return IPv4Addr::getNmaskFromInterface( name );
	}

	inline HwAddr NetworkInterface::getHwAddress( void )
		throw( std::system_error )
	{
		return HwAddr::getFromInterface( name );
	}
} // namespace reroman

#endif // REROMAN_NETWORKINTERFACE_HPP
