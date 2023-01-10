extends KinematicBody

var velocity : Vector3 = Vector3.ZERO
var snap_vector := Vector3.DOWN

export var speed := 3
export var jump_strenght := 11.0
export var gravity := 50.0
export var camera_sensitivity := 0.5

const MIN_CAMERA_ANGLE =- 70
const MAX_CAMERA_ANGLE = 40

onready var head : Spatial = $Head
onready var tween : Tween = $Tween

onready var life_label : Label = $UI/Life

var skin = "survivorMaleB"
var MASTER = false ## If this correspond to MY player

var life = 100

func _ready():
	$SpringArm.queue_free()
	
	var texture = load("res://src/player/animated-characters/Skins/" + skin + ".png")
	$Persona/Skeleton/character.material_override = SpatialMaterial.new()
	$Persona/Skeleton/character.material_override.albedo_texture = texture
	
	if MASTER:
		Input.set_mouse_mode(Input.MOUSE_MODE_CAPTURED)
	else:
		$Weapon.queue_free()
		$Head/Camera.current = false
		$Head/Camera.queue_free()
		$ViewportContainer.queue_free()
		$UI.queue_free()
	

func _physics_process(delta):
	if MASTER:
		var movement = _get_movement_direction().normalized()
		
		velocity.x = movement.x * speed
		velocity.z = movement.z * speed
		
		velocity.y -= gravity * delta
		velocity = move_and_slide(velocity, Vector3.UP)
	else:
		if not tween.is_active():
			move_and_slide(velocity * speed)
	
	if velocity.is_equal_approx(Vector3.ZERO) and $Persona/AnimationPlayer.assigned_animation != "idle":
		$Persona/AnimationPlayer.play("idle")
		
	elif not velocity.is_equal_approx(Vector3.ZERO):
		$Persona/AnimationPlayer.play("run")
		

func _process(delta):
	if MASTER:
		life_label.set_text(str(life))

func _input(event):
	if MASTER:
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

func take_damage(damage):
	life -= damage

func _on_Area_body_entered(body):
	if body.is_in_group("Enemies"):
		body._set_target(self)

func set_position(new_value):
	tween.interpolate_property(self, "global_transform:origin", global_transform.origin, new_value, 0.1)
	tween.start()

func _on_Network_tick_rate_timeout():
	if MASTER:
		global._send_P2P_Packet(0, {"ps": global_transform.origin, "rt": Vector2(global_rotation.y, head.rotation.x), "vl": velocity}, Steam.P2P_SEND_UNRELIABLE)
