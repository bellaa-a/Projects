function getId(student, callback) {
    console.log(`Fetch ${student} id`)
    let id;
    
    setTimeout(() => {
        id = "jdoe123";
        console.log(`Got ${student} id`)
        callback(id);
    }, 5000);
}


console.log("Listening to events");
const id = getId("John Doe", (id) => {
    console.log("id is " + id);
});

// const courses = getCourses(id);
// const grades = getGrades(id, courses);
console.log("Still listening to events");

function getCourses(student_id) {

}

function getGrades(student_id, student_courses) {

}

