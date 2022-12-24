extends Node


var attacking = false
var SPEED = 100
var motion = Vector2(0, 0)

var heroSelected = "Ray"
var hero

var loaded = false
var inputs = false
# Called when the node enters the scene tree for the first time.
func _ready():
	
	var format_hero_scene = "res://Player/heros/%s/%s.tscn"
	var actua_hero_scene = format_hero_scene % [heroSelected.to_lower(), heroSelected]
	hero = load(actua_hero_scene).instance()
	hero.inputs = inputs
	
	
	add_child(hero)
		
		
func _physics_process(delta):
	if inputs:
		move_from_input()
		
		
func move_from_input():
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
		
	hero.move_and_slide(motion, Vector2(0, 0), false, 4, 0.785, true)
	
	if hero.get_animation() != "idle":
		if !(Input.is_action_pressed("up") || Input.is_action_pressed("down") || Input.is_action_pressed("left") || Input.is_action_pressed("right")):
			hero.run_action("idle")
		motion = Vector2(0, 0)
		
	for index in hero.get_slide_count():
		var collision = hero.get_slide_collision(index)
		#print(collision)

func _unhandled_input(event):
	if inputs:
		if event is InputEventMouseButton:
			hero.attack()
		
		#EntitiesControler.print_tei()

func set_position(x, y):
	pass
	
func set_animation(animation):
	hero.run_action("run")
	pass
