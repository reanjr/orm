<?php
/**
 * Copyright Skinit, Inc.
 */

class Orm
{
    const ERROR_INVALID_PRIMARY_KEY = 'invalid primary key';
    
    /**
     * Generate model classes from the INFORMATION_SCHEMA of the provided database.
     * 
     * Config:
     * {
     *      "app": "catalog",
     *      "base_path": "models/orm/base",
     *      "user_path": "models/orm"
     * }
     * 
     * @param   ISchematic      $schematic
     * @param   array           $config
     */
    public static function generateModels( ISchematic $schematic, array $config )
    {
        /* @var $relation SchemaRelation */
        /* @var $relationship SchemaRelationship */

        $models = array();
        foreach( $schematic->getRelations() as $relation )
        {
            // create a new model builder for this relation and add it to the models list
            $model = new OrmModelBuilder();

            // generate model class names from the relation name
            $className = ucfirst( self::formatLogicalName( $relation ) ) . "Model";
            $baseClassName = $className . "Base";

            // add properties to the model builder for each of the relation attributes
            foreach( $relation->getAttributes() as $attribute )
            {
                $propertyName = self::formatLogicalName( $attribute );
                $dataType = $attribute->getDataType();
                $model->addProperty( $propertyName, $dataType );
            }

            // add properties for related models
            foreach( $relation->getRelationships() as $relationship )
            {
                // start with the name of the first attribute in the from key
                $fromAttribute = array_shift( $relationship->getFromAttributes() );
                $from = self::formatPhysicalName( $fromAttribute );

                // grab the name of the corresponding attribute in the to key
                $toAttribute = array_shift( $relationship->getToAttributes() );
                $to = self::formatPhysicalName( $toAttribute );

                // add new property by stripping "to" from end of "from"; ex: id, parent_id -> parent
                $propertyName = preg_replace( '@_' . $to . '^@', '', $from );
                $dataType = ucfirst( self::formatLogicalName( $relationship->getToRelation() ) ) . "Model";
                $model->addProperty( $propertyName, $dataType );
            }
        }
    }
    
    /**
     * Return a formatted physical name for the provided "physical_name", "logicalName", or "friendly name".
     * 
     * @param   string  $name
     * @return  string
     */
    public static function formatPhysicalName( $name )
    {
        // if there's an underscore in the name, assume it's already a physical name
        if( preg_match( '@_@', $name ) )
        {
            return $name;
        }
        
        // if there's a space in the name, assume it's in friendly format
        if( preg_match( '@ @', $name ) )
        {
            return strtolower( str_replace( ' ', '_', $name ) );
        }
        
        // otherwise, assume it's in logical format
        $logicalWords = preg_split( '@([[:upper:]][[:lower:]]+)@', $name, null,
                PREG_SPLIT_DELIM_CAPTURE|PREG_SPLIT_NO_EMPTY );     // identify Word as delimiter, capture words
        return strtolower( implode( '_', $logicalWords ) );
    }
    
    /**
     * Return a logical name for the provided "physical_name", "logicalName", or "friendly name".
     * 
     * @param   string  $name
     * @return  string 
     */
    public static function formatLogicalName( $name )
    {
        // if there's an underscore in the name, assume it's in physical name
        if( preg_match( '@_@', $name ) )
        {
            $physicalWords = explode( '_', strtolower($name) );
            return lcfirst( implode( '', array_map('ucfirst', $physicalWords) ) );
        }
        
        // if there's a space in the name, assume it's in friendly format
        if( preg_match( '@ @', $name ) )
        {
            $friendlyWords = explode( ' ', strtolower($name) );
            return lcfirst( implode( '', array_map('ucfirst', $friendlyWords) ) );
        }
        
        // otherwise, assume it's already in logical format
        return $name;
    }
    
    /**
     * Return a friendly name for the provided "physical_name", "logicalName", or "friendly name".
     * 
     * @param   string  $name
     * @return  string
     */
    public static function formatFriendlyName( $name )
    {
        // if there's an underscore in the name, assume it's in physical name
        if( preg_match( '@_@', $name ) )
        {
            $physicalWords = explode( '_', strtolower($name) );
            return ucwords( implode( ' ', $physicalWords ) );
        }
        
        // if there's a space in the name, assume it's already in friendly format
        if( preg_match( '@ @', $name ) )
        {
            return $name;
        }
        
        // otherwise, assume it's in logical format
        $logicalWords = preg_split( '@([[:upper:]][[:lower:]]+)@', $name, null,
                PREG_SPLIT_DELIM_CAPTURE|PREG_SPLIT_NO_EMPTY );     // identify Word as delimiter, capture words
        return ucwords( strtolower( implode( ' ', $logicalWords ) ) );
    }
    
}