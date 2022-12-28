extends Spatial
# Declare member variables here. Examples:
# var a = 2
# var b = "text"


# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.

func _physics_process(delta):
	$Camera.global_transform.origin.z = $KinematicBody.transform.origin.z
	$Camera.global_transform.origin.x = $KinematicBody.transform.origin.x
# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass
