extends Label
var score: int = 0


func _on_sprite_2d_burgerserved() -> void:
	score += 1
	text = "score: %d" % score
