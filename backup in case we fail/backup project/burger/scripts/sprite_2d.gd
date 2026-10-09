extends Sprite2D
var stage = 1
var skins: Array = [
	preload("res://burger/sprites/burger-14.png.png"),
	preload("res://burger/sprites/burger-15.png.png"),
	preload("res://burger/sprites/burger-16.png.png"),
	preload("res://burger/sprites/burger-17.png.png"),
	preload("res://burger/sprites/burger-18.png.png"),
	preload("res://burger/sprites/burger-19.png.png"),]
var burger_to_instance = preload("res://burger/models/full_burger.tscn")
var heel_to_instance = preload("res://burger/models/heel bun.tscn")
var lettuce_to_instance = preload("res://burger/models/lettuce.tscn")
var cheese_to_instance = preload("res://burger/models/cheese.tscn")
var patty_to_instance = preload("res://burger/models/patty.tscn")
var tomato_to_instance = preload("res://burger/models/tomato.tscn")
var crown_to_instance = preload("res://burger/models/crown bun.tscn")


func _on_area_2d_body_entered(body: Node2D) -> void:
	if stage == 1:
		if body.is_in_group("heel"):
			print("Player entered area:", body.name)
			body.queue_free()
			texture =skins[1]
			stage = 2
		else:
			print('test')
	if stage == 2:
		if body.is_in_group("patty"):
			print('yeah its a patty')
			body.queue_free()
			texture=skins[2]
			stage=3
	if stage == 3:
		if body.is_in_group("cheese"):
			print('yeah its a cheese')
			body.queue_free()
			texture=skins[3]
			stage=4
	if stage == 4:
		if body.is_in_group("tomato"):
			print('yeah its a tomato')
			body.queue_free()
			texture=skins[4]
			stage=5
	if stage == 5:
		if body.is_in_group("lettuce"):
			print('yeah its a lettuce')
			body.queue_free()
			texture=skins[5]
			stage=6
	if stage == 6:
		if body.is_in_group("crown"):
			print('yeah its a crow gang')
			body.queue_free()
			texture=skins[0]
			
			var borgar = burger_to_instance.instantiate()
			borgar.position = Vector2() # Set position
			add_child(borgar) # Add to scene tree
			var lettuce = lettuce_to_instance.instantiate()
			var cheese = cheese_to_instance.instantiate()
			var tomato = tomato_to_instance.instantiate()
			var patty = patty_to_instance.instantiate()
			var heel = heel_to_instance.instantiate()
			var crown = crown_to_instance.instantiate()
			lettuce.position = Vector2(randi_range(-100,100),randi_range(-100,100))
			cheese.position = Vector2(randi_range(-100,100),randi_range(-100,100))
			tomato.position = Vector2(randi_range(-100,100),randi_range(-100,100))
			patty.position = Vector2(randi_range(-100,100),randi_range(-100,100))
			heel.position = Vector2(randi_range(-100,100),randi_range(-100,100))
			crown.position = Vector2(randi_range(-100,100),randi_range(-100,100))
			get_parent().get_parent().add_child(lettuce)
			get_parent().get_parent().add_child(cheese)
			get_parent().get_parent().add_child(tomato)
			get_parent().get_parent().add_child(patty)
			get_parent().get_parent().add_child(heel)
			get_parent().get_parent().add_child(crown)
			stage=1
	
