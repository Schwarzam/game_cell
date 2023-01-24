extends Camera


export var camera_path : NodePath
var camera : Camera
var screen_size : Vector2 = Vector2()


func _ready():
	#screen_size = OS.get_screen_size()
	#print(get_parent())
	#get_parent().set_size(screen_size)
	camera = get_node(camera_path)


func _process(delta):
	#if OS.get_screen_size() != screen_size:
	#	screen_size = OS.get_screen_size()
	#	get_parent().set_size(screen_size)
	
	
	global_transform = camera.global_transform
	#global_transform.origin.y -= 0.16
	pass
	
