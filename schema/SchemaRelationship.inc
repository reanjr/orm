<?php
/**
 * Copyright Skinit, Inc.
 */

class SchemaRelationship
{
    protected $fromKey = null;
    protected $toKey = null;

    /**
     * Set the keys for this relationship.  Throw an exception if the keys are not the same size or if they have zero
     * size.
     *
     * @param   SchemaKey   $fromKey
     * @param   SchemaKey   $toKey
     */
    public function __construct( SchemaKey $fromKey, SchemaKey $toKey )
    {
        if( $fromKey->getSize() == 0 )
        {
            throw new Exception( "relationship key may not be empty" );
        }

        if( $fromKey->getSize() != $toKey->getSize() )
        {
            throw new Exception( "relationship keys must be the same size" );
        }

        $this->fromKey = $fromKey;
        $this->toKey = $toKey;
    }
    
}