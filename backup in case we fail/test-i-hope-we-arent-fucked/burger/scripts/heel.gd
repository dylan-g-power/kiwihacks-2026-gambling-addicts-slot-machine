extends RigidBody2D
## Zero-gravity floater. Drag with the left mouse button; let go mid-drag to fling it.

@export var start_speed := 80.0  # pixels per second
@export var max_speed := 300.0  # also limits flings
@export var bounce_off_screen_edges := true
@export var edge_margin := 32.0  # about the object's radius, in pixels

var dragging := false


func _ready() -> void:
	add_to_group("heel")
	input_pickable = true
	can_sleep = false  # stay awake so a stopped object can still be grabbed
	gravity_scale = 0.0
	linear_damp_mode = DAMP_MODE_REPLACE  # ignore the project's default damping
	angular_damp_mode = DAMP_MODE_REPLACE
	angular_damp = 0.5  # spin fades out gradually (higher = faster)
	physics_material_override = PhysicsMaterial.new()
	physics_material_override.bounce = 1.0
	physics_material_override.friction = 0.0
	linear_velocity = Vector2.from_angle(randf() * TAU) * start_speed * randf_range(0.5, 1.0)
	angular_velocity = randf_range(-3.0, 3.0)


func _input_event(_viewport: Viewport, event: InputEvent, _shape_idx: int) -> void:
	dragging = dragging or (event is InputEventMouseButton and event.pressed)


func _integrate_forces(state: PhysicsDirectBodyState2D) -> void:
	var vel := state.linear_velocity.limit_length(max_speed)
	var pos := state.transform.origin
	dragging = dragging and Input.is_mouse_button_pressed(MOUSE_BUTTON_LEFT)

	if dragging:  # steer toward the cursor; letting go keeps the velocity, so it flies off
		vel = (get_global_mouse_position() - pos) * 20.0
	elif bounce_off_screen_edges:
		var bounds := (get_canvas_transform().affine_inverse() * get_viewport_rect()).grow(-edge_margin)
		if pos.x < bounds.position.x: vel.x = absf(vel.x)
		if pos.x > bounds.end.x: vel.x = -absf(vel.x)
		if pos.y < bounds.position.y: vel.y = absf(vel.y)
		if pos.y > bounds.end.y: vel.y = -absf(vel.y)

	state.linear_velocity = vel
