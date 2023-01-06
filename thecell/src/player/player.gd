extends KinematicBody



var velocity : Vector3 = Vector3.ZERO
var snap_vector := Vector3.DOWN

export var speed := 3
export var jump_strenght := 11.0
export var gravity := 50.0
export var camera_sensitivity := 0.5


const MIN_CAMERA_ANGLE =- 60
const MAX_CAMERA_ANGLE = 70
onready var head : Spatial = $Head

func _ready():
	var texture = load("res://src/player/animated-characters/Skins/survivorMaleB.png")
	$Persona/Skeleton/character.material_override = SpatialMaterial.new()
	$Persona/Skeleton/character.material_override.albedo_texture = texture
	
	Input.set_mouse_mode(Input.MOUSE_MODE_CAPTURED)


func _physics_process(delta):
	var movement = _get_movement_direction().normalized()
	
	velocity.x = movement.x * speed
	velocity.z = movement.z * speed
	
	velocity.y -= gravity * delta
	
	#var just_landed := is_on_floor() and snap_vector == Vector3.ZERO
	#var is_jumping := is_on_floor() and Input.is_action_just_pressed("jump")
	
	#if is_jumping:
	#	velocity.y = jump_strenght
	#	snap_vector = Vector3.ZERO
	#elif just_landed:
	#	snap_vector = Vector3.DOWN
	
	#velocity = move_and_slide_with_snap(velocity, snap_vector, Vector3.UP, true)
	velocity = move_and_slide(velocity, Vector3.UP)


func _unhandled_input(event):
	if event is InputEventMouseMotion:
		_handle_camera_rotation(event)
		
		
		
func _handle_camera_rotation(event):
	rotate_y(deg2rad(-event.relative.x * camera_sensitivity))
	head.rotate_x(deg2rad(event.relative.y * camera_sensitivity))
	head.rotation.x = clamp(head.rotation.x, deg2rad(MIN_CAMERA_ANGLE), deg2rad(MAX_CAMERA_ANGLE))



func _get_movement_direction():
	var direction = Vector3.ZERO
	
	if Input.is_action_pressed("forward"):
		direction += transform.basis.z
	if Input.is_action_pressed("back"):
		direction -= transform.basis.z
	if Input.is_action_pressed("left"):
		direction += transform.basis.x
	if Input.is_action_pressed("right"):
		direction -= transform.basis.x
		
	return direction
	

func _on_Area_body_entered(body):
	if body.is_in_group("Enemies"):
		body._set_target(self)
