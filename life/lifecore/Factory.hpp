//@HEADER
/*
*******************************************************************************

Copyright (C) 2004, 2005, 2007 EPFL, Politecnico di Milano, INRIA
Copyright (C) 2010 EPFL, Politecnico di Milano, Emory University

This file is part of LifeV.

LifeV is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

LifeV is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with LifeV.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************
*/
//@HEADER
/*!
  @file
  @brief Factory class

  @date 4-10-2004
  @author Christophe Prud'homme <christophe.prudhomme@epfl.ch>

  @maintainer Radu Popescu <radu.popescu@epfl.ch>
*/

#ifndef FACTORY_H
#define FACTORY_H 1

#include <map>
#include <stdexcept>
#include <string>

#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-parameter"

#include <boost/function.hpp>
#include <boost/bind.hpp>

#pragma GCC diagnostic warning "-Wunused-variable"
#pragma GCC diagnostic warning "-Wunused-parameter"

#include <life/lifecore/debug.hpp>
#include <life/lifecore/typeInfo.hpp>

namespace LifeV
{

//! @struct FactoryDefaultError
/*!
  Manages the "Unknown Type" error in an object factory.
*/
//!\todo uncomment this line
//template <class AbstractProduct>
template <typename IdentifierType, class AbstractProduct>
struct FactoryDefaultError
{
    class Exception :
            public std::exception
    {
    public:
//!\todo uncomment this line
//        Exception( const std::string& id )
        Exception( IdentifierType id ) :
                std::exception(),
                M_exception()
        {
            std::ostringstream ex_str;
            //!\todo uncomment this line
            //ex_str << "[factory] Unknown Type : " + id;
            ex_str << "[factory] Unknown Type : ";
            M_exception = ex_str.str();

        }
        ~Exception() throw()
        {}
        const char* what() const throw ()
        {
            return M_exception.c_str();
        }
    private:
        std::string M_exception;
    };

    //!\todo uncomment this line
    //static AbstractProduct* onUnknownType(const std::string& id )
    static AbstractProduct* onUnknownType(IdentifierType id )
    {
        throw Exception( id );
    }
};

/*!
  @class factory
  @brief Implements a generic object factory

  @sa factoryDefaultError, factoryClone, FactoryTypeInfo
*/

//!\todo uncomment this line
// template <class AbstractProduct, typename IdentifierType,
//           typename ProductCreator = boost::function<AbstractProduct*()>,
//           template<class> class factoryErrorPolicy = FactoryDefaultError >
// class Factory : public FactoryErrorPolicy<AbstractProduct>

template <class AbstractProduct, typename IdentifierType,
          typename ProductCreator = boost::function<AbstractProduct*()>,
          template<typename, class> class FactoryErrorPolicy = FactoryDefaultError>
class Factory : public FactoryErrorPolicy<IdentifierType,AbstractProduct>
{
public:
    //! @name Public Typedefs
    //@{

    //!\todo uncomment this line
    //typedef std::string identifier_Type;
    typedef IdentifierType identifier_Type;
    typedef AbstractProduct product_Type;
    typedef ProductCreator creator_Type;
    //!\todo uncomment this line
    //typedef FactoryErrorPolicy<product_Type> super;
    typedef FactoryErrorPolicy<identifier_Type, product_Type> super;
    //@}

    //! @name Constructor and destructor
    //@{
    Factory() {}

    virtual ~Factory() {}
    //@}

    //! @name  Methods
    //@{

    /**
     * Register a product.
     *
     * A product is composed of an identifier (typically a
     * std::string) and a functor that will create the associated
     * object.
     *
     * @param id identifier for the object to be registered
     * @param creator the functor that will create the registered
     * object
     *
     * @return true if registration went fine, false otherwise
     */
    bool registerProduct( const identifier_Type& id, creator_Type creator )
    {
        LifeV::Debug( 2200 ) << "Registered type with id : " << id << "\n";
        return M_associations.insert( typename productId_Type::value_type( id, creator ) ).second;
    }

    /**
     * Unregister a product
     *
     * @param id
     * @sa registerProduct
     * @return true if unregistration went fine, false otherwise
     */
    bool unregisterProduct( const identifier_Type& id )
    {
        LifeV::Debug( 2200 ) << "Unregistered type with id : " << id << "\n";
        return M_associations.erase( id ) == 1;
    }

    /**
     * Create an object from a product registered in the factory using
     * identifier \c id
     *
     * @param id identifier of the product to instantiate
     *
     * @return the object associate with \c id
     */
    product_Type* createObject( const identifier_Type& id )
    {
        typename productId_Type::const_iterator i = M_associations.find( id );
        if (i != M_associations.end())
        {
            LifeV::Debug ( 2200 ) << "Creating type with id : " << id << "\n";
            return (i->second)();
        }
        LifeV::Debug( 2200 ) << "Unknown type with id : " << id << "\n";
        return super::onUnknownType( id );
    }
    //@}

private:
    typedef std::map<identifier_Type, creator_Type> productId_Type;
    productId_Type M_associations;
};

/*!
  @class FactoryClone
  @brief Implements a generic cloning object factory

  @sa Factory, FactoryDefaultError
*/
//!\todo uncomment this line
// template <class AbstractProduct,
//           class ProductCreator = boost::function<AbstractProduct* (const AbstractProduct*)>,
//           template<class> class FactoryErrorPolicy = FactoryDefaultError>
// class FactoryClone : public FactoryErrorPolicy<AbstractProduct>

template <class AbstractProduct,
          class ProductCreator = boost::function<AbstractProduct* (const AbstractProduct*)>,
          template<typename, class> class FactoryErrorPolicy = FactoryDefaultError>
class FactoryClone : public FactoryErrorPolicy<FactoryTypeInfo, AbstractProduct>
{
public:
    //! @name Typedefs
    //@{
    //!\todo uncomment this line
    //typedef FactoryErrorPolicy<AbstractProduct> super;
    typedef FactoryErrorPolicy<FactoryTypeInfo,AbstractProduct> super;
    //@}

    //! @name Constructor and destructor
    //@{
    FactoryClone() {}

    virtual ~FactoryClone() {}
    //@}

    //! @name  Methods
    //@{
    bool registerProduct(const FactoryTypeInfo& id, ProductCreator creator)
    {
        return M_associations.insert( typename productId_Type::value_type( id, creator ) ).second;
    }

    bool unregisterProduct( const FactoryTypeInfo& id )
    {
        return M_associations.erase(id) == 1;
    }

    AbstractProduct* createObject( const AbstractProduct* model )
    {
        if ( model == 0 ) return 0;

        typename productId_Type::const_iterator i = M_associations.find( typeid(*model) );
        if ( i != M_associations.end() )
        {
            return (i->second)(model);
        }
        //!\todo uncomment this line
        //return super::onUnknownType(typeid(*model).name());
        return super::onUnknownType(typeid(*model));
    }
    //@}

private:
    typedef std::map<FactoryTypeInfo, ProductCreator> productId_Type;
    productId_Type M_associations;
};

}
#endif // FACTORY_H