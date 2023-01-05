extends KinematicBody
# Declare member variables here. Examples:
# var a = 2
# var b = "text"

export var velocity : Vector3 = Vector3.ZERO
export var jump_strenght := 15.0
export var gravity := 50.0
export var camera_sensitivity := 0.5

const MIN_CAMERA_ANGLE =- 60
const MAX_CAMERA_ANGLE = 70
onready var head : Spatial = $Head

func _ready():
	Input.set_mouse_mode(Input.MOUSE_MODE_CAPTURED)


func _physics_process(delta):
	var movement = _get_movement_direction()
	velocity = movement * 2
	velocity = move_and_slide(velocity)



func _unhandled_input(event):
	if event is InputEventMouseMotion:
		_handle_camera_rotation(event)
		
		
		
func _handle_camera_rotation(event):
	rotate_y(deg2rad(-event.relative.x * camera_sensitivity))
	head.rotate_x(deg2rad(event.relative.y * camera_sensitivity))
	head.rotation.x = clamp(head.rotation.x, deg2rad(MIN_CAMERA_ANGLE), deg2rad(MAX_CAMERA_ANGLE))



func _get_movement_direction():
	var direction = Vector3.DOWN * 2
	
	if Input.is_action_pressed("forward"):
		direction += transform.basis.z
	if Input.is_action_pressed("back"):
		direction -= transform.basis.z
	if Input.is_action_pressed("left"):
		direction += transform.basis.x
	if Input.is_action_pressed("right"):
		direction -= transform.basis.x
		
	return direction
	
