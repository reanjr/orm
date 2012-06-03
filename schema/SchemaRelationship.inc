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

    /**
     * Return the "from" key.  Which side of the relationship is the "from" side and which is the "to" side depends on
     * the particular driver.  In general, if a relationship is defined in the schema on one side or another, the "from"
     * side is the one on which the relationship is defined or the one which is guaranteed to reference only a single
     * entity on the other side.
     *
     * @return  SchemaKey
     */
    public function getFromKey()
    {
        return $this->fromKey;
    }
    
    /**
     * Return the "to" key.  Which side of the relationship is the "to" side and which is the "from" side depends on
     * the particular driver.  In general, if a relationship is defined in the schema on one side or another, the "to"
     * side is the one on which the relationship is referenced by the other side or the one which is not guaranteed to
     * reference only a single entity on the other side.
     *
     * @return  SchemaKey
     */
    public function getToKey()
    {
        return $this->toKey;
    }

    /**
     * Return the relation for the "from" key.
     *
     * @return  SchemaRelation
     */
    public function getFromRelation()
    {
        return $this->getFromKey()->getRelation();
    }

    /**
     * Return the attribute for the "from" key.
     *
     * @return  array
     */
    public function getFromAttributes()
    {
        return $this->getFromKey()->getAttributes();
    }

    /**
     * Return the relation for the "to" key.
     *
     * @return  SchemaRelation
     */
    public function getToRelation()
    {
        return $this->getToKey()->getRelation();
    }

    /**
     * Return the attribute for the "to" key.
     *
     * @return  array
     */
    public function getToAttributes()
    {
        return $this->getToKey()->getAttributes();
    }

}