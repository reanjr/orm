<?php
/**
 * Copyright Skinit, Inc.
 */

class OrdersModelBase extends OrmModel
{
    protected $shippingAddressId = null;
    protected $shippingAddress = null;

    public function getConnection()
    {
        return SkinitOrm::APP_WEBSITE;
    }
    
    public function getSchema()
    {
        return 'web';
    }
    
    public function getRelation()
    {
        return 'orders';
    }
    
    public function getPrimaryKey()
    {
        return array( 'id' );
    }
    
    public function getUniqueKeys()
    {
        return array();
    }

    /**
     * Return the value of the shipping address id property.  This will auto-hydrate the model from the data store if
     * necessary.
     * 
     * @return  int
     */
    public function getShippingAddressId()
    {
        // simple property access
        return $this->getProperty( 'shippingAddressId' );
    }

    /**
     * Set the value of the shipping address id property.
     *
     * @param   int     $shippingAddressId
     */
    public function setShippingAddressId( $shippingAddressId )
    {
        
    }

    /**
     * Return the related shipping address based on the shipping address id property.
     *
     * @return  AddressesModel
     */
    public function getShippingAddress()
    {
        // related model access
        if( !isset($this->shippingAddress) )
        {
            $primaryKey = array();

            // add shipping address id to key; if we don't have one, return null
            if( is_null( $primaryKey[] = $this->getShippingAddressId() ) )
            {
                return null;
            }

            // use reflection to create a new instance of AddressesModel and set this property
            $reflector = new ReflectionClass( 'AddressesModel' );
            $this->shippingAddress = $reflector->newInstanceArgs( $pk );
        }

        // return the shipping address instance
        return $this->shippingAddress;
    }
    
}