<?php
/**
 * Copyright Skinit, Inc.
 */

class OrdersModelBase extends OrmModel
{
    
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

    public function getOrderLineItemId()
    {
        
    }
    
    public function getOrderLineItemList()
    {
        
    }
    
}