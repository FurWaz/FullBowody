let title = document.getElementById("main-title");
window.addEventListener("scroll", ev =>{
    let shift = window.scrollY/2;
    title.style.transform = "translateY("+shift+"px)";
});