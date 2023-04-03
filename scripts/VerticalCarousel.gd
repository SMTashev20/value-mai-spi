extends VBoxContainer

class_name VerticalCarousel

@onready var _actual_current_number = 1;

@onready var label_array: Array[Label] = []
@onready var current_number: int:
	get: return _actual_current_number
	set(val): _set_current_number(val)

signal number_changed(new_val, old_val)

func _set_current_number(val: int):
	_actual_current_number = val
	var counter = val - label_array.size() / 2
	for label in label_array:
		if counter <= 0: label.text = "000"
		else:
			label.text = ("000" + str(counter))
			label.text = label.text.substr(label.text.length() - 3, 3)
		counter += 1

# Called when the node enters the scene tree for the first time.
func _ready():
	for node in get_children():
		if node is Label:
			label_array.append(node)

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	pass

func _on_up_button_pressed():
	if current_number == 0: return
	current_number -= 1
	number_changed.emit(current_number, current_number + 1)

func _on_down_button_pressed():
	current_number += 1
	number_changed.emit(current_number, current_number - 1)
