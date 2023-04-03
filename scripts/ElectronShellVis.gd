extends Node2D

class_name ElectronShellVis

@onready var screen_size = get_viewport().get_visible_rect().size
@onready var screen_center = screen_size / 2.0

@export var ring_width: float
@export var spacing: float
@export var points_amount: int
@export var core_radius: float
@export var electron_radius: float

@export var core_color: Color
@export var electron_color: Color
@export var ring_color: Color

var shells: Array = [2, 8, 8, 2, 0, 0, 0, 0, 0, 0, 0]

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	pass

func _draw():
	draw_circle(screen_center, core_radius, core_color)
	
	var counter = 0
	for shell in shells:
		draw_arc(
			screen_center,
			spacing * (counter + 1) + core_radius, 0.,
			TAU, points_amount * (counter + 1),
			ring_color, ring_width, true)
		
		var rad_per_electron = TAU / shell
		for electron in shell:
			draw_circle(Vector2(
				screen_center.x + sin(rad_per_electron * electron) * spacing * (counter + 1) + core_radius,
				screen_center.y + cos(rad_per_electron * electron) * spacing * (counter + 1) + core_radius
			), electron_radius, electron_color)
		
		counter += 1
