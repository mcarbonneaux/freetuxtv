<?php
$this->breadcrumbs=array(
	'Web Streams'=>array('index'),
	$model->Name,
);
?>

<?php
	$this->menu=array();
	$this->menu[] = array('label'=>'View', 'url'=>array('view', 'id'=>$model->Id));
	if(Yii::app()->user->checkAccess('editWebStream')) {
		$this->menu[] = array('label'=>'Edit', 'url'=>array('update', 'id'=>$model->Id));
	}
	if(Yii::app()->user->checkAccess('changeStatusWebStream')) {
		$this->menu[] = array('label'=>'Change status', 'url'=>array('changestatus', 'id'=>$model->Id));
	}
/*
$this->menu=array(
	array('label'=>'List WebStream', 'url'=>array('index')),
	array('label'=>'Create WebStream', 'url'=>array('create')),
	array('label'=>'Update WebStream', 'url'=>array('update', 'id'=>$model->Id)),
	array('label'=>'Delete WebStream', 'url'=>'#', 'linkOptions'=>array('submit'=>array('delete','id'=>$model->Id),'confirm'=>'Are you sure you want to delete this item?')),
	array('label'=>'Manage WebStream', 'url'=>array('admin')),
);*/

?>

<h1>WebStream : <?php echo $model->Name; ?></h1>

Here you can see the detail of the channel <?php echo $model->Name; ?> :

<br><br>

<?php $this->widget('zii.widgets.CDetailView', array(
	'data'=>$model,
	'attributes'=>array(
		array(
            'label'=>'Id',
			'name'=>'Id',
            'type'=>'text',
        ),
		'Name',
        array(
            'label'=>'Type',
			'type'=>'html',
            'value'=>"<font>".$model->getTypeStreamName()."</font>",
        ),
		array(
            'label'=>'Url',
			'name'=>'Url',
            'type'=>'url',
        ),
		'RequiredIsp',
		array(
            'label'=>'Language',
			'type'=>'html',
            'value'=>($model->LangCode ? '<img src="'.Yii::app()->request->baseUrl.'/images/lang/languageicons/flags/'.strtolower($model->LangCode).'.png'.'"> '.$model->Lang->Label : $model->LangCode),
        ),
        array(
            'label'=>'Status',
			'type'=>'html',
            'value'=>'<font color="'.$model->StreamStatus->Color.'">'.$model->StreamStatus->Label.'</font>',
        ),
		'SubmissionDate',
	),
)); ?>

<br><br>

<?php echo $this->renderPartial('_viewhistory', array('dataHistory'=>$dataHistory)); ?>