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
 * @brief Declaración de clases y funciones relacionadas con el 
 * procotolo ARP.
 */

#ifndef REROMAN_ARP_HPP
#define REROMAN_ARP_HPP

#include <reroman/networkinterface.hpp>


namespace reroman
{
	/**
	 * @brief Define tipos y enumeraciones para el protocolo
	 * ARP.
	 */
	namespace arp
	{
		class ARPSocket;

		/**
		 * @brief Agrega una entrada ethernet estática a la cache ARP del
		 * sistema.
		 * @details Modificar la cache del sistema requiere permisos de superusuario.
		 * @param nic Interfaz de red a la cual agregar la nueva entrada.
		 * @param ip Dirección IP a la cual pertenece el registro.
		 * @param hw Dirección física a la cual está asociada la IP.
		 * @return Verdadero si la entrada se agregó con éxito, falso en caso
		 * contrario.
		 */
		bool addStaticSystemEntry( const reroman::NetworkInterface &nic,
				const IPv4Addr &ip, const reroman::HwAddr &hw );

		/**
		 * @brief Elimina una entrada de la cache ARP del sistema.
		 * @details Modificar la cache del sistema requiere permisos de superusuario.
		 * @param nic Interfaz de red de la cual eliminar el registro.
		 * @param ip Dirección IP de la entrada a eliminar.
		 * @return Verdadero si la entrada se eliminó con éxito, falso en caso
		 * contrario.
		 */
		bool delSystemEntry( const reroman::NetworkInterface &nic,
				const reroman::IPv4Addr &ip );

		/**
		 * @brief Obtiene una dirección MAC desde la cache del sistema dada
		 * una dirección IP.
		 * @param nic Interfaz de red asociada al registro.
		 * @param ip Dirección IP de la cual se desea conocer la dirección física.
		 * @return La dirección física en cache de la dirección IP.
		 * @throw std::invalid_argument Si la interfaz de red no es válida.
		 * @throw std::out_of_range Si la IP no se encuentra en la caché del sistema.
		 * @throw std::system_error Si ocurriese alǵún error.
		 */
		reroman::HwAddr getSystemEntry( const reroman::NetworkInterface &nic,
				const reroman::IPv4Addr &ip );

		/**
		 * @brief Códigos de operación para el protocolo ARP.
		 */
		enum class OperationCode: uint16_t
		{
			REQUEST	= 1,	///< Petición ARP.
			REPLY			///< Respuesta ARP.
		};

		/**
		 * @brief Tipos de hardware para el protocolo ARP.
		 */
		enum class HwType: uint16_t
		{
			NETROM,				///< From KA9Q: NET/ROM pseudo.	 
			ETHER,				///< Ethernet 10/100Mbps.  		 
			EETHER,				///< Ethernet Experimental.  		 
			AX25,				///< AX.25 Nivel 2.  	 	 	 	 
			PRONET,				///< PROnet token ring.  	 	 	 
			CHAOS,				///< Chaosnet.  	 	 	 		 
			IEEE802,			///< IEEE 802.2 Ethernet/TR/TB.  	 
			ARCNET,				///< ARCnet.  	 	 	 		 
			APPLETLK,			///< APPLEtalk.  	 	 	 		 
			DLCI		=	15, ///< Frame Relay DLCI.  	 	 	 
			ATM			=	19, ///< ATM.  	 	 	 		 	 
			METRICOM	=	23, ///< Metricom STRIP (new IANA id). 
			IEEE1394,			///< IEEE 1394 IPv4 - RFC 2734.  	 
			EUI64		=	27, ///< EUI-64.  	 	 	 		 
			INFINIBAND	=	32	///< InfiniBand.  	 	 	 	
		};

		/**
		 * @brief Protocolos soportados. Actualmente sólo IPv4
		 */
		enum class Protocol: uint16_t
		{
			IPV4 = 0x0800 ///< Protocolo IPv4
		};

		/**
		 * @brief Define una trama ARP
		 * @headerfile arp.hpp <reroman/arp/arp.hpp>
		 */
		class __attribute__((packed)) ARPFrame final
		{
			friend class ARPSocket;
		public:
			//===============================================================
			//							Constructores
			//===============================================================
			/**
			 * @brief Crea una trama vacía con algunos valores por defecto.
			 * @details La nueva trama es una petición por defecto para redes
			 * ethernet e IPv4, con direcciones de origen y destino nulas.
			 * @param op Código de operación de la trama. Por defecto es una petición.
			 * @param hw Tipo de hardware de la trama. Por defecto Ethernet 10/100 Mbps.
			 */
			ARPFrame( OperationCode op = OperationCode::REQUEST, HwType hw = HwType::ETHER );


			//===============================================================
			//							Getters
			//===============================================================
			/**
			 * @brief Obtiene el tipo de hardware de la trama.
			 */
			HwType getHwType( void ) const noexcept;

			/**
			 * @brief Obtiene el tipo de protocolo.
			 */
			Protocol getProtocol( void ) const noexcept;

			/**
			 * @brief Obtiene la longitud de la dirección de hardware.
			 */
			uint8_t getHwLen( void ) const noexcept;

			/**
			 * @brief Obtiene la longitud de la dirección de protocolo.
			 */
			uint8_t getProtocolLen( void ) const noexcept;

			/**
			 * @brief Obtiene el código de operación
			 */
			OperationCode getOpCode( void ) const noexcept;

			/**
			 * @brief Obtiene una referencia a la dirección física de origen.
			 */
			reroman::HwAddr getSourceHwAddr( void ) const noexcept;

			/**
			 * @brief Obtiene una referencia a la dirección IP de origen.
			 */
			reroman::IPv4Addr getSourceIPAddr( void ) const noexcept;

			/**
			 * @brief Obtiene una referencia a la dirección física destino.
			 */
			reroman::HwAddr getTargetHwAddr( void ) const noexcept;

			/**
			 * @brief Obtiene una referencia a la dirección IP destino.
			 */
			reroman::IPv4Addr getTargetIPAddr( void ) const noexcept;


			//===============================================================
			//							Setters
			//===============================================================
			/**
			 * @brief Establece el tipo de hardware.
			 */
			void setHwType( HwType hw ) noexcept;

			/**
			 * @brief Establece el formato de protocolo.
			 */
			void setProtocol( Protocol p ) noexcept;

			/**
			 * @brief Establece la longitud de la dirección física.
			 */
			void setHwLen( uint8_t len ) noexcept;

			/**
			 * @brief Establece la longitud de la dirección de protocolo.
			 */
			void setProtocolLen( uint8_t len ) noexcept;

			/**
			 * @brief Establece el código de operación.
			 */
			void setOpCode( OperationCode op ) noexcept;

			/**
			 * @brief Establece la dirección física de origen.
			 */
			void setSourceHwAddr( const reroman::HwAddr &addr );

			/**
			 * @brief Establece la dirección de protocolo de origen.
			 */
			void setSourceIPAddr( const reroman::IPv4Addr &addr ) noexcept;

			/**
			 * @brief Establece la dirección física destino.
			 */
			void setTargetHwAddr( const reroman::HwAddr &addr );

			/**
			 * @brief Establece la dirección de protocolo de origen.
			 */
			void setTargetIPAddr( const reroman::IPv4Addr &addr ) noexcept;

		private:
			uint16_t hwType;
			uint16_t protocol;
			uint8_t hwlen;
			uint8_t iplen;
			uint16_t opcode;
			uint8_t hwSrc[reroman::HwAddr::HwAddrLen];
			uint32_t ipSrc;
			uint8_t hwTgt[reroman::HwAddr::HwAddrLen];
			uint32_t ipTgt;
		};

		/**
		 * @brief Representa un socket para enviar/recibir tramas ARP.
		 * @headerfile arp.hpp <reroman/arp/arp.hpp>
		 */
		class ARPSocket final
		{
		public:
			//===============================================================
			//							Constructores
			//===============================================================
			/**
			 * @brief Abre un nuevo socket filtrando tramas ARP.
			 * @details Sólo procesos con id de usuario efectivo 0 o capacidad
			 * CAP_NET_RAW pueden crear este tipo de sockets.
			 * @param msecs Tiempo máximo en ms para esperar por una
			 * respuesta. Si es establecido a 0 entonces se esperará
			 * hasta que haya una respuesta.
			 * @throw std::system_error si no puede abrirse el socket.
			 */
			explicit ARPSocket( unsigned int msecs = 100 );

			~ARPSocket();


			//===============================================================
			//							Getters
			//===============================================================
			/**
			 * @brief Obtiene el tiempo máximo de espera para recibir una trama.
			 * @return Un entero que indica el tiempo en milisegundos.
			 */
			int getTimeout( void ) const noexcept;

			//===============================================================
			//							Setters
			//===============================================================
			/**
			 * @brief Establece el tiempo máximo de espera para recibir una trama.
			 * @param msecs El tiempo en milisegundos a establecer.
			 * @return Verdadero si la acción se completó con éxito, falso en caso
			 * de error. En caso de error el valor actual se conserva.
			 */
			bool setTimeout( unsigned int msecs );

			//===============================================================
			//							Operadores
			//===============================================================
			/**
			 * @brief Mueve los datos de un objeto ARPSocket a otro
			 * @param sock Objeto a mover.
			 * @return una referencia este objeto.
			 */
			ARPSocket& operator=( ARPSocket &&sock );


			//===============================================================
			//							Operaciones
			//===============================================================
			/**
			 * @brief Recibe una trama ARP.
			 * @param[out] frame Trama en la cual se almacenará el resultado.
			 * @param[out] sender Si no es null, almacena la dirección del remitente.
			 * @return Verdadero si se leyó el frame, falso si terminó el tiempo
			 * de espera antes de leer algo.
			 * @throw std::system_error si ocurriera algún error.
			 */
			bool receive( ARPFrame &frame, reroman::HwAddr *sender = nullptr );

			/**
			 * @brief Envia una trama ARP.
			 * @param frame La trama que se desea enviar.
			 * @param dest Dirección a la cual enviar la trama.
			 * @param nic Interfaz de red por la cual enviar la trama.
			 * @return Verdadero si la trama se envió correctamente, falso
			 * si ocurrió algún error cambiando el valor de errno.
			 */
			bool send( const ARPFrame &frame, const reroman::HwAddr &dest,
				   const reroman::NetworkInterface &nic	);

			/**
			 * @brief Enlaza el socket a una interfaz de red (sólo para recibir).
			 * @param nic Interfaz a la cual se desea enlazar el socket.
			 * @return Verdadero si se enlazó con éxito, falso en caso contrario
			 * estableciendo el valor de errno.
			 */
			bool bind( const reroman::NetworkInterface &nic );


			/**
			 * @brief Resuelve una dirección IP utilizando el protocolo.
			 * @param ip Dirección IP que se desea resolver.
			 * @param nic Interfaz de red a utilizar.
			 * @param[out] result Si no es null, almacena la dirección física
			 * asociada.
			 * @return Verdadero si la resolución pudo hacerse, falso en caso
			 * contrario.
			 * @throw system_error si ocurre algún error.
			 */
			bool resolve( const reroman::IPv4Addr &ip, 
					const reroman::NetworkInterface &nic,
					reroman::HwAddr *result = nullptr );

		private:
			int sock;
			struct timeval timer;
		};


		//===============================================================
		//					Métodos Inline	
		//===============================================================
		inline ARPFrame::ARPFrame( OperationCode op, HwType hw )
			: hwType( htons(static_cast<unsigned short>(hw)) ),
			protocol( htons(static_cast<unsigned short>(Protocol::IPV4)) ),
			hwlen( HwAddr::HwAddrLen ),
			iplen( IPv4Addr::IPv4AddrLen ),
			opcode( htons(static_cast<unsigned short>(op)) ){}

		inline HwType ARPFrame::getHwType( void ) const noexcept
		{
			return static_cast<HwType>( ntohs(hwType) );
		}

		inline Protocol ARPFrame::getProtocol( void ) const noexcept
		{
			return static_cast<Protocol>( ntohs(protocol) );
		}

		inline uint8_t ARPFrame::getHwLen( void ) const noexcept
		{
			return hwlen;
		}

		inline uint8_t ARPFrame::getProtocolLen( void ) const noexcept
		{
			return iplen;
		}

		inline OperationCode ARPFrame::getOpCode( void ) const noexcept
		{
			return static_cast<OperationCode>( ntohs(opcode) );
		}

		inline HwAddr ARPFrame::getSourceHwAddr( void ) const noexcept
		{
			return reroman::HwAddr( hwSrc );
		}

		inline IPv4Addr ARPFrame::getSourceIPAddr( void ) const noexcept
		{
			return reroman::IPv4Addr( ipSrc );
		}

		inline HwAddr ARPFrame::getTargetHwAddr( void ) const noexcept
		{
			return reroman::HwAddr( hwTgt );
		}

		inline IPv4Addr ARPFrame::getTargetIPAddr( void ) const noexcept
		{
			return reroman::IPv4Addr( ipTgt );
		}

		inline void ARPFrame::setHwType( HwType hw ) noexcept
		{
			hwType = htons( static_cast<unsigned short>(hw) );
		}

		inline void ARPFrame::setProtocol( Protocol p ) noexcept
		{
			protocol = htons( static_cast<unsigned short>(p) );
		}

		inline void ARPFrame::setHwLen( uint8_t len ) noexcept
		{
			hwlen = len;
		}

		inline void ARPFrame::setProtocolLen( uint8_t len ) noexcept
		{
			iplen = len;
		}

		inline void ARPFrame::setOpCode( OperationCode op ) noexcept
		{
			opcode = htons( static_cast<unsigned short>(op) );
		}

		inline void ARPFrame::setSourceHwAddr( const HwAddr &addr )
		{
			addr.copyTo( hwSrc );
		}

		inline void ARPFrame::setSourceIPAddr( const IPv4Addr &addr ) noexcept
		{
			ipSrc = addr.toNetworkInt();
		}

		inline void ARPFrame::setTargetHwAddr( const HwAddr &addr )
		{
			addr.copyTo( hwTgt );
		}

		inline void ARPFrame::setTargetIPAddr( const IPv4Addr &addr ) noexcept
		{
			ipTgt = addr.toNetworkInt();
		}


		inline int ARPSocket::getTimeout( void ) const noexcept
		{
			return timer.tv_sec * 1000 +
				timer.tv_usec / 1000;
		}
	} // namespace arp
} // namespace reroman

#endif // ARP_HPP
