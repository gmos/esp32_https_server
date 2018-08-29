/*
 * HTTPRequest.cpp
 *
 *  Created on: Dec 13, 2017
 *      Author: frank
 */

#include "HTTPRequest.hpp"

namespace httpsserver {

HTTPRequest::HTTPRequest(ConnectionContext * con, HTTPHeaders * headers, ResourceParameters * params, std::string requestString, std::string method):
	_con(con),
	_headers(headers),
	_params(params),
	_requestString(requestString),
	_method(method) {

	HTTPHeader * contentLength = headers->get("Content-Length");
	if (contentLength == NULL) {
		_remainingContent = 0;
		_contentLengthSet = false;
	} else {
		_remainingContent = parseInt(contentLength->_value);
		_contentLengthSet = true;
	}

}

HTTPRequest::~HTTPRequest() {

}


ResourceParameters * HTTPRequest::getParams() {
	return _params;
}

std::string HTTPRequest::getHeader(std::string name) {
	HTTPHeader * h = _headers->get(name);
	if (h != NULL) {
		return h->_value;
	} else {
		return std::string();
	}
}

std::string HTTPRequest::getRequestString() {
	return _requestString;
}

std::string HTTPRequest::getMethod() {
	return _method;
}

size_t HTTPRequest::readBytes(byte * buffer, size_t length) {

	// Limit reading to content length
	if (_contentLengthSet && length > _remainingContent) {
		length = _remainingContent;
	}

	size_t bytesRead = _con->readBuffer(buffer, length);

	if (_contentLengthSet) {
		_remainingContent -= bytesRead;
	}

	return bytesRead;
}

size_t HTTPRequest::readChars(char * buffer, size_t length) {
	return readBytes((byte*)buffer, length);
}

size_t HTTPRequest::getContentLength() {
	return _remainingContent;
}

bool HTTPRequest::requestComplete() {
	if (_contentLengthSet) {
		// If we have a content size, rely on it.
		return (_remainingContent == 0);
	} else {
		// If there is no more input...
		return (_con->pendingBufferSize() == 0);
	}
}

/**
 * This function will drop whatever is remaining of the request body
 */
void HTTPRequest::discardRequestBody() {
	byte buf[16];
	while(!requestComplete()) {
		readBytes(buf, 16);
	}
}

} /* namespace httpsserver */
