extends Sprite2D
var stage = 1
var skins: Array = [
	preload("res://burger/sprites/burger-14.png.png"),
	preload("res://burger/sprites/burger-15.png.png"),
	preload("res://burger/sprites/burger-16.png.png"),
	preload("res://burger/sprites/burger-17.png.png"),
	preload("res://burger/sprites/burger-18.png.png"),
	preload("res://burger/sprites/burger-19.png.png"),]
var scene_to_instance = preload("res://burger/models/full_burger.tscn")

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
			print('yeah its a crown')
			body.queue_free()
			texture=skins[0]
			stage=1
			var borgar = scene_to_instance.instantiate()
			borgar.position = Vector2() # Set position
			add_child(borgar) # Add to scene tree
	
