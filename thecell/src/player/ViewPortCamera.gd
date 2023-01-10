extends Camera


export var camera_path : NodePath
var camera : Camera

func _ready():
	camera = get_node(camera_path)
	

func _process(delta):
	global_transform = camera.global_transform
	#global_transform.origin.y -= 0.16
	pass
	#global_transform.origin.y
	
