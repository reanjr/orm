<?php
/**
 * Copyright Skinit, Inc.
 */

class OrdersModelBase extends OrmModel
{
    
    public function getApplication()
    {
        return Orm::APP_WEBSITE;
    }
    
    public function getSchema()
    {
        return 'web';
    }
    
    public function getTable()
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