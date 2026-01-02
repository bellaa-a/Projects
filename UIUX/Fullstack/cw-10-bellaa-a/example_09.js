function getId(student) {
    return new Promise((resolve, reject) => {
        console.log(`Fetch ${student} id`)
        let id;
        let success;

        setTimeout(() => {
            success = true;
            id = "jdoe123";
            
            if (success) {
                console.log(`Got ${student} id`)
                resolve(id);
            } else {
                const error = new Error(`Unable to fetch ${student} id`)
                reject(error);
            }
        }, 1000);
    });
}

function getCourses(student_id) {
    return new Promise((resolve, reject) => {
        console.log(`Fetch ${student_id} courses`)
        let success, courses;
        
        setTimeout(() => {
            success = true;
            courses = ["course-1", "course-2"];
            if (success) {
                console.log(`Got ${student_id} courses`)
                resolve(courses);
            } else {
                reject(new Error(`Unable to get ${student_id} courses`))
            }
        }, 1000);
    });
}

function getGrades(student_id, student_courses) {
    return new Promise((resolve, reject) => {
        console.log(`Fetch ${student_id} grades`)
        let success, grades;
        
        setTimeout(() => {
            success = true;
            grades = student_courses.map((course) => {
                return {
                    course,
                    grade: Math.floor(Math.random() * 100)
                }
            })

            if (success) {
                console.log(`Got ${student_id} grades`)
                resolve(grades);
            } else {
                reject(new Error(`Unable to get ${student_id} grades`))
            }
        }, 1000);
    })
}

async function getStudentGradesAndDisplayIt() {
    try {
        const id = await getId("John Doe");
        const courses = await getCourses(id);
        const grades = await getGrades(id, courses);
        console.log(grades);
    } catch(err) {
        console.error(err)
    }
}

console.log("Listening to events");

getStudentGradesAndDisplayIt();

console.log("Still listening to events");

