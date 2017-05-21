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
		 * @brief Crea un objeto sin estar asociado a alguna interfaz de red.
		 */
		NetworkInterface( void ) = default;

		/**
		 * @brief Crea un objeto y lo asocia a una interfaz de red en el sistema
		 * a partir de su nombre.
		 * @param ifname Nombre de la interfaz de red.
		 * @throw std::system_error si el nombre de la interfaz no existe.
		 */
		explicit NetworkInterface( std::string ifname );

		/**
		 * @brief Obtiene los datos de una interfaz de red en el sistema
		 * a partir de su índice.
		 * @param index Índice de la interfaz.
		 * @throw std::system_error si el índice de la interfaz no existe.
		 */
		explicit NetworkInterface( int index );


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
		IPv4Addr getAddress( void ) const;

		/**
		 * @brief Obtiene la máscara de subred asignada a la interfaz.
		 * @return Un objeto IPv4Addr con la máscara de subred.
		 * @throw std::system_error si no se puede obtener la dirección
		 * requerida.
		 */
		IPv4Addr getNetmask( void ) const;

		/**
		 * @brief Obtiene la dirección física asignada a la interfaz.
		 * @return Un objeto HwAddr con la dirección de hardware del dispositivo.
		 * @throw std::system_error si no se puede obtener la dirección
		 * requerida.
		 */
		HwAddr getHwAddress( void ) const;

		/**
		 * @brief Verifica si la interfaz está en modo promiscuo.
		 * @return Verdadero si se encuentra en modo promiscuo, falso
		 * en caso contrario.
		 * @throw std::system_error si ocurre algún error.
		 */
		bool isPromiscModeEnabled( void ) const;

		/**
		 * @brief Determina si el objeto se encuentra asociado a una interfaz
		 * de red en el sistema.
		 * @return Verdadero si se encuentra asociado, falso en caso contrario.
		 */
		bool isBinded( void ) const;


		//===============================================================
		//							Setters
		//===============================================================
		/**
		 * @brief Activa/desactiva el modo promiscuo.
		 * @param value Un valor verdadero activa el modo promiscuo, un
		 * valor falso lo desactiva.
		 * @return Verdadero si la acción se completó con éxito, falso
		 * en caso contrario.
		 */
		bool setPromiscMode( bool value );

		/**
		 * @brief Asocia el objeto a una interfaz de red en el sistema.
		 * @param ifname Nombre de la interfaz de red.
		 * @return Verdadero si se hizo la asociación, falso en caso contrario.
		 */
		bool bind( std::string ifname );

		/**
		 * @brief Asocia el objeto a una interfaz de red en el sistema.
		 * @param index Índice de la interfaz de red.
		 * @return Verdadero si se hizo la asociación, falso en caso contrario.
		 */
		bool bind( int index );

	private:
		std::string name;
		int index;
		bool binded = false;
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

	inline IPv4Addr NetworkInterface::getAddress( void ) const
	{
		return IPv4Addr::getFromInterface( name );
	}

	inline IPv4Addr NetworkInterface::getNetmask( void ) const
	{
		return IPv4Addr::getNmaskFromInterface( name );
	}

	inline HwAddr NetworkInterface::getHwAddress( void ) const
	{
		return HwAddr::getFromInterface( name );
	}

	inline bool NetworkInterface::isBinded( void ) const
	{
		return binded;
	}
} // namespace reroman

#endif // REROMAN_NETWORKINTERFACE_HPP
