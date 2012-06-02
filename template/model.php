<?phpecho "<?php"?>
/**
 * Copyright Skinit, Inc.
 */

class <?php echo $model->getClassName() ?>Base
{

    public function getSchema()
    {
        return '<?php echo $model->getSchema->getName() ?>';
    }

    public function getRelation()
    {
        return '<?php echo $model->getRelation()->getName() ?>';
    }

    public function getPrimaryKey()
    {
        return array(
<?php foreach( $model->getPrimaryKey() as $key ): ?>
            '<?php echo $key ?>',
<?php endforeach ?>
        );
    }

}