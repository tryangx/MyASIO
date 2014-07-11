#pragma once

#include "XAsioTCP.h"
#include <exception>

using namespace XASIO;

class CServer
{
	boost::asio::io_service		m_ioService;
	TcpServerPtr				m_ptrTcpServer;
	TcpSessionPtr				m_ptrTCPSession;

	int					m_iPort;
	boost::thread		m_thread;
	bool				m_bRun;
public:
	CServer() : m_bRun( false ) {}

	void	init( int port )
	{
		if ( m_bRun )
		{
			return;
		}
		m_bRun = true;

		m_iPort = port;
		m_ptrTcpServer = XAsioTCPServer::create( m_ioService );
		m_ptrTcpServer->setAcceptHandler( &CServer::onAccept, this );
		m_ptrTcpServer->setCancelHandler( &CServer::onCancel, this );
		m_ptrTcpServer->setErrorHandler( &CServer::onError, this );

		accept();

		m_thread = boost::thread( boost::bind(&CServer::run, this) );
	}

	void close()
	{
		m_ptrTcpServer->cancel();
	}

	void run()
	{
		TRACE("run server\n");
		try
		{
			m_ioService.run();
		}
		catch ( const std::exception& e )
		{
			TRACE( "quit" );
		}		
	}

	void	accept()
	{
		if ( m_ptrTcpServer )
		{
			m_ptrTcpServer->startAccept( 2, m_iPort );
		}
	}

	void	onAccept( TcpSessionPtr	session )
	{
		m_ptrTCPSession = session;
		m_ptrTCPSession->setCloseHandler( [ & ]() 
		{ 
			TRACE( "Server close\n" );
		} );
		m_ptrTCPSession->setErrorHandler( &CServer::onError, this );
		m_ptrTCPSession->setReadCompleteHandler( &CServer::onReadComplete, this );
		m_ptrTCPSession->setReadHandler( &CServer::onRead, this );
		m_ptrTCPSession->setWriteHandler( &CServer::onWrite, this );

		// Start reading data from the client. 
		m_ptrTCPSession->read();
	}

	void	onCancel()
	{
		TRACE( "Server cancel\n" );
		accept();
	}
	void	onClose()
	{
		TRACE( "Server close\n" );
		accept();
	}
	void	onError( std::string str, size_t bytesTransferred )
	{
		str = "Server Error:" + str + "\n";
		TRACE( str.c_str() );
	}
	void	onRead( XAsioBuffer buffer )
	{
		TRACE( "Server read %d\n", buffer.getDataSize() );

		// Data is packaged as a ci::Buffer. This allows 
		// you to send any kind of data. Because it's more common to
		// work with strings, the session object has static convenience 
		// methods for converting between std::string and ci::Buffer.
		std::string response = XAsioSessionInterface::bufferToString( buffer );
		TRACE( "%s\n", response.c_str() );

		// We're simply echoing what the client sent us. Write the response
		// back to them to acknowledge the succesful communication.
		m_ptrTCPSession->write( XAsioTCPSession::stringToBuffer( response ) );
	}
	void	onReadComplete()
	{
		TRACE( "Server Read complete\n" );

		// Close the session.
		m_ptrTCPSession->close();

		// Cancel any remaining IO operations (there shouldn't be any).
		// This will tell the server to start listening for the next 
		// connection. See the onCancel method.
		m_ptrTcpServer->cancel();
	}
	void	onWrite( size_t bytesTransferred )
	{
		// Read after writing to look for an EOF, or disconnect signal
		// from the client.
		m_ptrTCPSession->read();
	}
};

class CClient
{
	boost::asio::io_service		m_ioService;
	TcpClientPtr				m_ptrTCPClient;
	TcpSessionPtr				m_ptrTCPSession;

	int					m_iPort;
	std::string			m_sHost;
	std::string			m_sRequest;

	bool				m_bRun;
	boost::thread		m_thread;
public:
	CClient() : m_bRun( false ) {}

	void		init( int port )
	{
		if ( m_bRun )
		{
			return;
		}
		m_bRun = true;

		m_sHost = "localhost";
		m_iPort = port;
		m_sRequest = "echo";

		// Initialize a client by passing a boost::asio::io_service to it.
		// ci::App already has one that it polls on update, so we'll use that.
		// You can use your own io_service, but you will have to manage it 
		// manually (i.e., call poll(), poll_one(), run(), etc).
		m_ptrTCPClient = XAsioTCPClient::create( m_ioService );

		// Add callbacks to work with the client asynchronously.
		// Note that you can use lambdas.
		m_ptrTCPClient->setConnectHandler( &CClient::onConnect, this );
		m_ptrTCPClient->setErrorHandler( &CClient::onError, this );
		m_ptrTCPClient->setResolveHandler( [ & ]()
		{
			TRACE( "Client Endpoint resolved\n" );
		} );
		
		m_ptrTCPClient->connect( m_sHost, m_iPort );

		m_thread = boost::thread( boost::bind(&CClient::run, this) );
	}
	void		close()
	{
		if ( m_ptrTCPSession )
		{
			m_ptrTCPSession->close();
		}
	}
	void		run()
	{
		TRACE( "run client\n" );
		
		try
		{
			m_ioService.run();
		}
		catch ( const std::exception& e )
		{
			TRACE( "quit" );
		}
	}

	void		write()
	{
		// This sample is meant to work with only one session at a time.
		if ( m_ptrTCPSession && m_ptrTCPSession->getSocket()->is_open() ) 
		{
			// Write data is packaged as a ci::Buffer. This allows 
			// you to send any kind of data. Because it's more common to
			// work with strings, the session object has static convenience 
			// methods for converting between std::string and ci::Buffer.
			XAsioBuffer buffer = XAsioTCPSession::stringToBuffer( m_sRequest );
			m_ptrTCPSession->write( buffer );
		}
		else 
		{	
			// Before we can write, we need to establish a connection 
			// and create a session. Check out the onConnect method.
			//std::string s = boost::lexical_cast<std::string>( m_iPort );
			//TRACE( "Connecting to: " + m_sHost + ":" + s );
			m_ptrTCPClient->connect( m_sHost, (uint16_t)m_iPort );
		}
	}

	void		onConnect( TcpSessionPtr session )
	{
		TRACE( "Client Connected\n" );

		// Get the session from the argument and set callbacks.
		// Note that you can use lambdas.
		m_ptrTCPSession = session;
		m_ptrTCPSession->setCloseHandler( [ & ]()
		{
			TRACE( "Client Disconnected\n" );
		} );
		m_ptrTCPSession->setErrorHandler( &CClient::onError, this );
		m_ptrTCPSession->setReadCompleteHandler( [ & ]()
		{
			TRACE( "Client Read complete\n" );
		} );
		m_ptrTCPSession->setReadHandler( &CClient::onRead, this );
		m_ptrTCPSession->setWriteHandler( &CClient::onWrite, this );

		write();
	}
	void		onError( std::string err, size_t bytesTransferred )
	{
		std::string str = "Client error:" + err;
		TRACE( "%s\n", str.c_str() );
	}
	void		onRead( XAsioBuffer& buffer )
	{
		std::string str = boost::lexical_cast<std::string>( buffer.getDataSize() ) + "bytes read";

		// Responses are passed in the read callback as ci::Buffer. Use
		// a convenience method on the session object to convert it to
		// a std::string.
		std::string response = XAsioTCPSession::bufferToString( buffer );
		TRACE( "Client read %s\n", response.c_str() );

		// Closing the connection after reading mimics the behavior
		// of a HTTP client. To keep the connection open and continue
		// communicating with the server, comment the line below.
		m_ptrTCPSession->close();
	}
	void		onWrite( size_t bytesTransferred )
	{
		std::string s = boost::lexical_cast<std::string>( bytesTransferred );
		std::string err = "Client write" + s;
		TRACE( "%s\n", err.c_str() );
		// After successfully sending your request to the server, you 
		// should expect a response. Start reading immediately.
		m_ptrTCPSession->read();
	}
};