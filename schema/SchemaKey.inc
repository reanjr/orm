<?php
/**
 * Copyright Skinit, Inc.
 */

class SchemaKey
{
    protected $attributes = array();

    /**
     * Set the attributes for this key.  Throw an exception if the schema or relation of any of the attributes do not
     * match or if no attributes are provided.
     *
     * @param   array
     */
    public function __construct( $attributes )
    {
        $firstAttribute = array_shift( $attributes );
        if( !$firstAttribute )
        {
            throw new Exception( "SchemaKey must have at least one attribute" );
        }

        // now validate any remaining attributes against the first and add them to the key
        $this->attributes[] = $firstAttribute;
        foreach( $attributes as $attribute )
        {
            $schemasMatch = (bool)$attribute->getSchema()->getName() == $firstAttribute->getSchema()->getName();
            $relationsMatch = (bool)$attribute->getRelation()->getName() == $firstAttribute->getRelation()->getName();
            if( !($schemasMatch && $relationsMatch) )
            {
                throw new Exception( "attributes must all have the same schema and relation" );
            }
            $this->attributes[] = $attribute;
        }
    }

    /**
     * Return the attributes that make up this key.
     *
     * @return  array
     */
    public function getAttributes()
    {
        return $this->attributes;
    }

    /**
     * Return the names of the attributes that make up this key.
     *
     * @return  array
     */
    public function getAttributeNames()
    {
        return array_map( function($value) { return $value->getName(); }, $this->getAttributes() );
    }

    /**
     * Return the number of attributes that make up this key.
     *
     * @return  int
     */
    public function getSize()
    {
        return count($this->getAttributes());
    }

    /**
     * Return the relation on which the key attributes are defined.
     *
     * @return  SchemaRelation
     */
    public function getRelation()
    {
        return $this->attributes[0]->getRelation();
    }
    
}