extends Sprite2D
signal burgerserved()
var skins: Array = [
	preload("res://sprites_aliens0.png"),
	preload("res://sprites_aliens1.png"),
	preload("res://sprites_aliens2.png"),
	preload("res://sprites_aliens3.png"),
	preload("res://sprites_aliens4.png"),
	preload("res://sprites_aliens5.png"),]



func _on_area_2d_body_entered(body: Node2D) -> void:
	if body.is_in_group('burger'):
		body.queue_free()
		burgerserved.emit()
		texture = skins.pick_random()
		
