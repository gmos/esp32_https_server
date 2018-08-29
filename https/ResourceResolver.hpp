/*
 * ResourceResolver.hpp
 *
 *  Created on: Dec 12, 2017
 *      Author: frank
 */

#ifndef HTTPS_RESOURCERESOLVER_HPP_
#define HTTPS_RESOURCERESOLVER_HPP_

#include <string>
// Arduino declares it's own min max, incompatible with the stl...
#undef min
#undef max
#include <vector>
#include <algorithm>

#include "ResourceNode.hpp"
#include "ResolvedResource.hpp"
#include "HTTPSMiddlewareFunction.hpp"

namespace httpsserver {

class ResourceResolver {
public:
	ResourceResolver();
	~ResourceResolver();

	void registerNode(ResourceNode *node);
	void unregisterNode(ResourceNode *node);
	void setDefaultNode(ResourceNode *node);
	void resolveNode(const std::string &method, const std::string &url, ResolvedResource &resolvedResource);

	/** Add a middleware function to the end of the middleware function chain. See HTTPSMiddlewareFunction.hpp for details. */
	void addMiddleware(const HTTPSMiddlewareFunction * mwFunction);
	/** Remove a specific function from the middleware function chain. */
	void removeMiddleware(const HTTPSMiddlewareFunction * mwFunction);
	/** Get the current middleware chain with a resource function at the end */
	const std::vector<HTTPSMiddlewareFunction*> getMiddleware();

private:

	// This vector holds all nodes (with callbacks) that are registered
	std::vector<ResourceNode*> * _nodes;
	ResourceNode * _defaultNode;
	// Middleware functions, if any are registered. Will be called in order of the vector.
	std::vector<const HTTPSMiddlewareFunction*> _middleware;
};

} /* namespace httpsserver */

#endif /* HTTPS_RESOURCERESOLVER_HPP_ */
