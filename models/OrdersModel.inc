<?php
/**
 * Copyright Skinit, Inc.
 */

class OrdersModel extends OrdersModelBase
{

    /**
     * Return an array of related order line items records.
     *
     * @return  array
     */
    public function getOrderLineItemsList()
    {
        return $this->getPrimaryKeyRelated( 'order_line_items' );
    }
    
}