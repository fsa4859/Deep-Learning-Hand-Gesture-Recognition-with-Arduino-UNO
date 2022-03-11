  // **********

  let portName = 'COM__'; // serial port
  let imageModelURL = 'https://teachablemachine.withgoogle.com/models/______/'; // model URL

  // **********

  let serial;
  let classifier;
  let video;
  let flippedVideo;
  let label = "";
  function preload() {
    classifier = ml5.imageClassifier(imageModelURL + 'model.json');
  }

  function setup() {
    serial = new p5.SerialPort();
    serial.list();
    serial.open(portName);
    serial.on('list', gotList);
    createCanvas(320, 260);
    video = createCapture(VIDEO);
    video.size(320, 240);
    video.hide();
    flippedVideo = ml5.flipImage(video)
    classifyVideo();
  }

  function draw() {
    background(0);
    image(flippedVideo, 0, 0);
    fill(255);
    textSize(16);
    textAlign(CENTER);
    text("Result: " + label, width / 2, height - 4);
  }

  function classifyVideo() {
    flippedVideo = ml5.flipImage(video)
    classifier.classify(flippedVideo, gotResult);
  }

  function gotResult(error, results) {
    if (error) {
      console.error(error);
      return;
    }
    label = results[0].label;
    print("Result:", String(results[0].label))
    serial.write(String(results[0].label));
    classifyVideo();
  }

  function gotList(thelist) {
    console.log("List of Serial Ports:");
    for (let i = 0; i < thelist.length; i++) {
      console.log(i + " " + thelist[i]);
    }
  }