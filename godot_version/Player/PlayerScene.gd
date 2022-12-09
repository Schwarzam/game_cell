extends KinematicBody2D


onready var _animated_sprite = $AnimatedSprite

var attacking = false
var SPEED = 100
var motion = Vector2(0, 0)

var heroSelected = "Ray"
var hero

var loaded = false
# Called when the node enters the scene tree for the first time.
func _ready():
	
	var format_hero_scene = "res://Player/heros/%s/%s.tscn"
	var actua_hero_scene = format_hero_scene % [heroSelected.to_lower(), heroSelected]
	print(actua_hero_scene, load(actua_hero_scene))
	hero = load(actua_hero_scene).instance()
	
	add_child(hero)

func _on_attack_finished():
	self.attacking = false
		
		
func _physics_process(delta):
		
	if Input.is_action_pressed("up"):
		motion.y -= SPEED
		hero.run_action("run")
	if Input.is_action_pressed("down"):
		motion.y += SPEED
		hero.run_action("run")
	if Input.is_action_pressed("right"):
		motion.x += SPEED
		hero.run_action("run")
	if Input.is_action_pressed("left"):
		motion.x -= SPEED
		hero.run_action("run")
		
	move_and_slide(motion, Vector2(0, 0), false, 4, 0.785, true)
	
	if hero.get_animation() != "idle":
		if !(Input.is_action_pressed("up") || Input.is_action_pressed("down") || Input.is_action_pressed("left") || Input.is_action_pressed("right")):
			hero.run_action("idle")
		motion = Vector2(0, 0)
		
	for index in get_slide_count():
		var collision = get_slide_collision(index)
		#print(collision)

func _unhandled_input(event):
	if event is InputEventMouseButton:
		hero.attack()
		
		#EntitiesControler.print_tei()
		
