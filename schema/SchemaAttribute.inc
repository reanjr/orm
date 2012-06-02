<?php
/**
 * Copyright Skinit, Inc.
 */

class SchemaAttribute
{
    protected $schema = null;
    protected $relationName = null;
    protected $name = null;
    protected $dataType = null;

    /**
     * Set the schema and the name of the relation in which this attribute is found.  Set the name of this attribute
     * and the data type, which should be one of the ISchematic::TYPE_* constants.
     *
     * @param   ISchematic  $schema
     * @param   string      $relationName
     * @param   string      $name
     * @param   string      $dataType
     */
    public function __construct( ISchematic $schema, $relationName, $name, $dataType )
    {
        $this->schema = $schema;
        $this->relationName = (string)$relationName;
        $this->name = (string)$name;
        $this->dataType = (string)$dataType;
    }

    /**
     * Return the schema in which this attribute is found.
     *
     * @return  ISchematic
     */
    public function getSchema()
    {
        return $this->schema;
    }

    /**
     * Return the relation on which this attribute is found.
     *
     * @return  SchemaRelation
     */
    public function getRelation()
    {
        $schemaRelations = $this->getSchema()->getRelations();
        foreach( $schemaRelations as $relation )
        {
            if( $relation->getName() == $this->relationName )
            {
                return $relation;
            }
        }
    }

    /**
     * Return the name of this attribute.
     *
     * @return  string
     */
    public function getName()
    {
        return $this->name;
    }

    /**
     * Return the data type of this attribute.  This should be on of the ISchematic::TYPE_* constants.
     *
     * @return  string
     */
    public function getDataType()
    {
        return $this->dataType;
    }
    
}