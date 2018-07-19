var x = document.createElement("test");
x.innerHTML = "Hello there";
document.getElementsByTagName("body")[0].appendChild(x);
setInterval(() => {
    window.location.reload(true);
}, 3000);
