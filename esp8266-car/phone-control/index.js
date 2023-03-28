function select(val) {
  return document.querySelector(val);
}

var joystick = new Joystick({
  zone: select('#left')
})
.init();

joystick.onStart = function(distance, angle) {
  console.log('Left:向 => ' + angle + '移动' + distance + '个单位');
  
  switch(angle) {
    case 'up':
      taggleCmd(angle, distance)
      break;
    case 'right':
      taggleCmd(angle, distance)
      break;
    case 'left':
      taggleCmd(angle, distance);
      break;
    case 'down':
      taggleCmd(angle, distance);
  }
}

joystick.onEnd = function() {
	taggleCmd("stop", "0")
}
