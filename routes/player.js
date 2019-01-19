const express = require('express')
const router = express.Router()
var fs = require('fs');
var bodyParser =	require("body-parser");
var multer	=	require('multer');
const path = require('path')

function getInstituteId(institute_name){
    var id= 0
     let existquery = "SELECT id FROM `institute` WHERE name='"+institute_name+"'"; 
    db.query(existquery, (err, result) => {
         
            if(result.length > 0){
                console.log("instituteid",result)
               id = result[0].id; 
            }
    })
    return id;
 
}
function getProgramId(program_name){
    var id= 0
     let existquery = "SELECT id FROM `programs` WHERE program_name='"+program_name+"'"; 
    db.query(existquery, (err, result) => {
         
            if(result.length > 0){
                 console.log("progid",result)
                id= result[0].id
            }
        return id
    })
    console.log("afterprog",id)
   
 
}
router.get('/gd',(req,res)=>{

            res.render('how_it_work.ejs', {
                title: "Welcome to Socka | View Players"
               
            })
    }),
router.get('/add',(req,res)=>{
let query = "SELECT * FROM `players` ORDER BY id ASC"; // query database to get all the players
//
        // execute query
        db.query(query, (err, data) => {
            if (err) {
                res.redirect('/');
            }
            res.render('add.ejs', {
                title: "Welcome to Socka | View Players"
                ,message: ""
                
            });
        });
    }),
    
router.get('/add-institute',(req,res)=>{

            res.render('add_institute.ejs', {
                title: "Welcome to Socka | View Players"
                ,message: ""
            })
    }),
router.get('/life-in-canada',(req,res)=>{
//let query = "SELECT * FROM `life_in_canada`"; // query database to get all the players
////
//        // execute query
//        db.query(query, (err, data) => {
//            console.log(data)
//            if (err) {
//                res.redirect('/');
//            }
            res.render('life_in_canada.ejs', {
                title: "Welcome to Socka | View Players"
                ,message: ""
            })
           
        }),
  
           
   
router.get('/edit-how-it-work',(req,res)=>{
 let query = "SELECT * FROM `how_it_works` "; // query database to get all the players
//  co
    
        // execute query
        db.query(query, (err, data) => {
            console.log(data)
            if (err) {
                res.redirect('/');
            }
  res.render('how_it_work.ejs', {
                title: "Welcome to Socka | View Players"
                ,message: "",hw:data
            })
        });
           
    }),
router.get('/edit-aboutus',(req,res)=>{
 let query = "SELECT * FROM `aboutuspage` "; // query database to get all the players
//  co
    
        // execute query
        db.query(query, (err, data) => {
            console.log(data)
            if (err) {
                res.redirect('/');
            }
  res.render('edit_about_us.ejs', {
                title: "Welcome to Socka | View Players"
                ,message: "",aboutus:data
            })
        });
           
    }),
router.get('/edit-life-in-canada',(req,res)=>{
 let query = "SELECT * FROM `life-in-canada` "; // query database to get all the players
//  co
    
        // execute query
        db.query(query, (err, data) => {
            console.log(data)
            if (err) {
                res.redirect('/');
            }
  res.render('edit-life-in-canada.ejs', {
                title: "Welcome to Socka | View Players"
                ,message: "",life:data
            })
        });
           
    }),
    router.get('/aboutus',(req,res)=>{
 let query = "SELECT * FROM `aboutuspage` "; // query database to get all the players
    let query2 = "SELECT * FROM `our_services` "; 
    let query3 = "SELECT * FROM `testimonial` "; 
//  co
    
        // execute query
        db.query(query, (err, data) => {
            console.log(data)
            if (err) {
                res.redirect('/');
            }
             db.query(query3, (err, data3) => {
            console.log(data)
            if (err) {
                res.redirect('/');
            }
             db.query(query2, (err, data2) => {
            console.log(data)
            if (err) {
                res.redirect('/');
            }
  res.render('aboutus.ejs', {
                title: "Welcome to Socka | View Players"
                ,message: "",aboutus:data,service:data2,testimonial:data3
            })
        });
             })
        })
           
    }),
        router.get('/search',(req,res)=>{
 let query = "SELECT id,name FROM `institute` "; // query database to get all the players
    let query2 = "SELECT id,program_name FROM `programs` "; 
   
    
        // execute query
        db.query(query, (err, data) => {
            console.log(data)
            if (err) {
                res.redirect('/');
            }
             db.query(query2, (err, data2) => {
            console.log(data2)
            if (err) {
                res.redirect('/');
            }
  res.render('search.ejs', {
                title: "Welcome to Socka | View Players"
                ,message: "",data:data,data2:data2
            })
        });
             })
        })
           
    ,
    router.get('/edit-testimonial',(req,res)=>{
 let query = "SELECT * FROM `testimonial` "; // query database to get all the players
//  co
    
        // execute query
        db.query(query, (err, data) => {
            console.log(data)
            if (err) {
                res.redirect('/');
            }
  res.render('testimonial.ejs', {
                title: "Welcome to Socka | View Players"
                ,message: "",ts:data
            })
        });
           
    }),
        router.get('/edit-our-service',(req,res)=>{
 let query = "SELECT * FROM `our_Services` "; // query database to get all the players
//  co
    
        // execute query
        db.query(query, (err, data) => {
            console.log(data)
            if (err) {
                res.redirect('/');
            }
  res.render('edit_our_service.ejs', {
                title: "Welcome to Socka | View Players"
                ,message: "",sv:data
            })
        });
           
    }),
    router.get('/edit-university-in-canada',(req,res)=>{ //for why study in caanda
    let query = "SELECT * FROM `study_in_university` "; // query database to get all the players
//  
        // execute query
        db.query(query, (err, data) => {
            console.log(data)
            if (err) {
                res.redirect('/');
            }
            res.render('study_in_university.ejs', {
                title: "Welcome to Socka | View Players"
                ,message: "",university:data
            })
        });
           
    }),
        router.get('/edit-college-in-canada',(req,res)=>{ //for why study in caanda
    let query = "SELECT * FROM `study_in_college` "; // query database to get all the players
//  
        // execute query
        db.query(query, (err, data) => {
            console.log(data)
            if (err) {
                res.redirect('/');
            }
            res.render('study_in_college.ejs', {
                title: "Welcome to Socka | View Players"
                ,message: "",college:data
            })
        });
           
    }),
    router.get('/admin',(req,res)=>{

            res.render('admin_login.ejs', {
                title: "Welcome to Socka | View Players"
                ,message: ""
            })
    }),
      router.get('/aboutus-description',(req,res)=>{
    let query = "SELECT * FROM `aboutus` "; // query database to get all the players
//  co
    
        // execute query
        db.query(query, (err, data) => {
            console.log(data)
            if (err) {
                res.redirect('/');
            }
  res.render('about_us_two_desc.ejs', {
                title: "Welcome to Socka | View Players"
                ,message: "",content:data
            })
        });
           
    }),
          
  
router.get('/student_request',(req,res)=>{
let query = "SELECT student_id ,name,Area_of_interest,status FROM `form` "; // query database to get all the players
//  co
    
        // execute query
        db.query(query, (err, data) => {
            console.log(data)
            if (err) {
                res.redirect('/');
            }
          res.render('manage_student_request.ejs', {
                title: "Welcome to Socka | View Players",requests:data
                
            })
        });
           
    }),
    router.get('/student_request/:id',(req,res)=>{
 let query1 = "SELECT * FROM `form` WHERE student_id='"+req.params.id+"'"; 
    // query database to get all the players
//  co
    
        // execute query
        db.query(query1, (err, data) => {
            console.log(data)
            if (err) {
                res.redirect('/');
            }
          res.render('student_request_detail.ejs', {
                title: "Welcome to Socka | View Players",requests:data
                
            })
        });
           
    }),
    
  
    router.get('/eligibility-form/:result',(req,res)=>{
           var message = "";
    var status = ""
    if(req.params.result == "success"){
        message = "Form has been submitted successfully, You will be contacted within 24 hours by one of our agent, Thank you"
        status = "success"
    }
    else{
        status = "new"
    }
        let query = "SELECT program_name FROM `programs` ORDER BY program_name"; // query database to get all the players
//
        // execute query
        db.query(query, (err, data) => {
            console.log(data)
            if (err) {
                res.redirect('/');
            }
             res.render('form.ejs', {
                title: "Welcome to Socka | View Players",message: message,status:status,programs:data
                
            })
        });
     
          
    }),
 router.get('/uploads',(req,res)=>{

            res.render('upload.ejs', {
                title: "Welcome to Socka | View Players"
               
            })
    }),
 router.get('/faq',(req,res)=>{
 console.log("H")
    let query = "SELECT id,question,answer FROM faq"
           db.query(query, (err, data) => {
            console.log(data)
            if (err) {
                res.redirect('/');
            }
            if(data.length > 0){
                res.render('faq.ejs', {
                title: "Welcome to Socka | View Players",data:data
               
            })
            }
          
    })
    }),
 router.get('/',(req,res)=>{

            res.redirect('/home')
    }),
     router.get('/home',(req,res)=>{
    let query = "SELECT program_name FROM `programs` ORDER BY program_name";
    let query2 = "SELECT * FROM `aboutus` "; // query database to get all the players
    let query3 = "SELECT * FROM `study_in_university` ";
     let query4 = "SELECT * FROM `study_in_college` ";
   let query5 = "SELECT * FROM `how_it_works` ";
    let query6 = "SELECT * FROM `testimonial` ";
      
        // execute query
    
    
        db.query(query2, (err, data2) => {
            //console.log(data2)
            if (err) {
                res.redirect('/');
            }
        db.query(query, (err, data) => {
           console.log(data)
            if (err) {
                res.redirect('/');
            }
              db.query(query4, (err, data4) => {
          //  console.log(data)
            if (err) {
                res.redirect('/');
            }
             db.query(query3, (err, data3) => {
            console.log(data3)
            if (err) {
                res.redirect('/');
            }
               db.query(query5, (err, data5) => {
            console.log(data5)
            if (err) {
                res.redirect('/');
            }
             db.query(query6, (err, data6) => {
            console.log(data6)
            if (err) {
                res.redirect('/');
            }
            res.render('mainpage.ejs', {
                title: "Welcome to Socka | View Players"
               , programname:data,content:data2,uni:data3,col:data4,hw:data5,ts:data6
            })
        });
        })
              })
              })
        })
    })
}),
         
router.get('/add-program',(req,res)=>{

            res.render('add_program.ejs', {
                title: "Welcome to Socka | View Players"
                ,message: ""
            })
    }),
router.get('/how-it-works',(req,res)=>{

            res.render('how.ejs', {
                title: "Welcome to Socka | View Players"
                ,message: ""
            })
    }),
router.get('/view-more-schools/:id/:name',(req,res)=>{

           console.log(req.params.id)
     console.log(req.params.name)
  let query1 = "SELECT program_id, institute_id,type ,programRanking,tuitionFees,videolink,septemberIntake,JanuaryIntake,JuneIntake,name,province,description,city,country FROM `institute_program` INNER JOIN `institute` ON institute_program.institute_id = institute.id WHERE program_id='"+req.params.id+"'";  
    
        db.query(query1, (err, Pdata) => {
            console.log(Pdata)
            if (err) {
                res.redirect('/');
            }
            if(Pdata.length > 0){
                res.render('view_more_schools.ejs', {
                title: "Welcome to Socka | View Players",Pdata:Pdata,details:req.params.name
               
            })
            }
          
    })
}),
    router.get('/program/:name',(req,res)=>{

     let query3 = "SELECT * FROM `how_it_works` ";
         let query1 = "SELECT * FROM `programs` WHERE program_name='"+req.params.name+"'"; 
    
        db.query(query1, (err, data) => {
            console.log("yeh",data)
            if (err) {
                res.redirect('/');
            }
            if(data.length > 0){
                
        let programquery = "SELECT id FROM `programs` WHERE program_name='"+req.params.name+"'"; 
    db.query(programquery, (err, pid) => {
 
            if(pid.length > 0){
                 console.log("progid",pid[0].id)
        let query2 = "SELECT program_id, institute_id,type ,programRanking,tuitionFees,videolink,septemberIntake,JanuaryIntake,JuneIntake,name,province,description,city,country FROM `institute_program` INNER JOIN `institute` ON institute_program.institute_id = institute.id WHERE program_id='"+pid[0].id+"'";  
                  db.query(query2, (err, Pdata) => {
                      console.log("pdata",Pdata)
                      
                          db.query(query3, (err, Hdata) => {
       
            if (err) {
                res.redirect('/');
            }
            if(data.length > 0){
                         res.render('programs.ejs', {
                title: "Welcome to Socka | View Players",details:data,Pdata:Pdata,Hdata:Hdata
               
            })
            }
                  })
                
            })
            }
                  
              })   
              
                
            }
        
        });
           
    }),
    
        router.get('/view_all_institute/:name',(req,res)=>{ //change so many info called
    console.log(req.params.name)
         let query1 = "SELECT * FROM `programs` WHERE program_name='"+req.params.name+"'"; 
    
        db.query(query1, (err, data) => {
            console.log(data)
            if (err) {
                res.redirect('/');
            }
            if(data.length > 0){
                
        let programquery = "SELECT id FROM `programs` WHERE program_name='"+req.params.name+"'"; 
    db.query(programquery, (err, pid) => {
 
            if(pid.length > 0){
                 console.log("progid",pid[0].id)
        let query2 = "SELECT program_id, institute_id,programRanking,tuitionFees,septemberIntake,JanuaryIntake,JuneIntake,name,province,description,city,country FROM `institute_program` INNER JOIN `institute` ON institute_program.institute_id = institute.id WHERE program_id='"+pid[0].id+"'";  
                  db.query(query2, (err, Pdata) => {
                      console.log("pdata",Pdata)
                         res.render('student_request_institutes.ejs', {
                title: "Welcome to Socka | View Players",details:data,requests:Pdata,degree:req.params.name
               
            })
                  })
                
            }
                  
              })   
              
                
            }
        
        });
           
    }),
    
        router.get('/view_all_institute_m/:name',(req,res)=>{ //change so many info called
    var id = (req.params.name).match(/\d+/g).map(Number)
    var name = (req.params.name).replace(id,'')
     name = name.trim()
    console.log(name)
                        let programquery = "SELECT id FROM `programs` WHERE program_name='"+name+"'"; 
    db.query(programquery, (err, pid) => {
 
            if(pid.length > 0){
                 console.log("progid",pid[0].id)
        let query2 = "SELECT program_id, institute_id,programRanking,tuitionFees,OUACregistration,applicationDeadline,supportingDocDeadline,applicationFee,IELTS,TOEFL,IB_english_test,iletsScore,toeflOnlineScore,toeflPaperScore,IBEnglishScore,oalevel,otherGrade,specialRequirement,IB_degree,american,septemberIntake,JanuaryIntake,JuneIntake,name,province,description,city,country FROM `institute_program` INNER JOIN `institute` ON institute_program.institute_id = institute.id WHERE program_id='"+pid[0].id+"' and institute_id='"+id+"'";  
                  db.query(query2, (err, Pdata) => {
                      console.log("pdata",Pdata)
                        res.render('./student_request_institutes_detail.ejs', {
                title: "Welcome to Socka | View Players",uni:Pdata
            })
                  })
                
            }
                  
              }) 
                  
               
         
              
                
           
        
        
           
    }),
    router.get('/add-institute-program',(req,res)=>{
  let existquery = "SELECT name FROM `institute` "; // query database 
     let programquery = "SELECT program_name FROM `programs` "; // query database 
    db.query(existquery, (err, result) => {
            if (err) {
                res.redirect('/');
            }
           db.query(programquery, (err, result2) => {
            if (err) {
                res.redirect('/');
            }
            console.log("institue",result2)
            res.render('add_program_for_institute.ejs', {
                title: "Welcome to Socka | View Players",result:result,result2:result2,message:""
                
            })
           })
    })
    }),
router.post('/add',(req,res)=>{
  console.log(req.body)  
    let existquery = "SELECT * FROM `players` WHERE first_name='"+req.body.name+"'"; // query database to get all the players
//
        // execute query
        db.query(existquery, (err, result) => {
            if (err) {
                res.redirect('/');
            }
            if(result.length > 0){
                console.log("result",result.length)
               res.render('add.ejs', {
                title: "Welcome to Socka | View Players"
                ,message: 'User with this name already exists'
                ,data:""
            });  
            }
            else{
                if(req.body.name != "" && req.body.last != "122"){
                 let query = "INSERT INTO `players` (first_name, last_name) VALUES ('" +req.body.name + "', '" + req.body.last + "')";
                        db.query(query, (err, result) => {
                            if (err) {
                                return res.status(500).send(err);
                            }
                            res.redirect('/add');    
            })
            }}
                    
        });
})

router.post('/edit-university-in-canada',(req,res)=>{
  console.log(req.body)  

            
                if(req.body.adduniversitypoint != ""){
                 let query = "INSERT INTO `study_in_university` (point) VALUES ('" +req.body.adduniversitypoint + "')";
                        db.query(query, (err, result) => {
                            if (err) {
                                return res.status(500).send(err);
                            }
                            res.redirect('/edit-university-in-canada');    
           
           
                        })
        }
    else{
        res.redirect('/edit-university-in-canada');  
    }
})
router.post('/edit-college-in-canada',(req,res)=>{
  console.log(req.body)  

            
                if(req.body.adduniversitypoint != ""){
                 let query = "INSERT INTO `study_in_college` (point) VALUES ('" +req.body.adduniversitypoint + "')";
                        db.query(query, (err, result) => {
                            if (err) {
                                return res.status(500).send(err);
                            }
                            res.redirect('/edit-college-in-canada');    
           
           
                        })
        }
    else{
        res.redirect('/edit-university-in-canada');  
    }
})
router.post('/edit-how-it-work',(req,res)=>{
  console.log(req.body)  

            
                 let query = "INSERT INTO `how_it_works` (heading, description,icon) VALUES ('" +req.body.stepsheading + "','" +req.body.stepsdescription + "', '" + req.body.stepsicon + "')";
                        db.query(query, (err, result) => {
                            if (err) {
                                return res.status(500).send(err);
                            }
                            res.redirect('/edit-how-it-work');    
           
           
                        })
        
  
})

router.post('/edit-our-service',(req,res)=>{
  console.log(req.body)  

            
                 let query = "INSERT INTO `our_services` (heading, service,icon) VALUES ('" +req.body.heading + "','" +req.body.service + "','" +req.body.icon + "')";
                        db.query(query, (err, result) => {
                            if (err) {
                                return res.status(500).send(err);
                            }
                            res.redirect('/edit-our-service');    
           
           
                        })
        
  
})
router.post('/search',(req,res)=>{
    var result = req.body.search
    if(req.body.Un){
        result = req.body.Un
    }
    if(req.body.province){
        result = req.body.province
    }
    if(req.body.institute){
        result = req.body.institute
    }
    if(req.body.term && req.body.programs){
        res.redirect("search-result-program/"+req.body.term+"/"+req.body.programs) 
    }
    
  console.log(req.body)  
res.redirect("search-result/"+result) 

    
  
}),
    router.get('/search-result-program/:term/:program',(req,res)=>{
        let query =  "SELECT program_id, institute_id,programRanking,tuitionFees,OUACregistration,applicationDeadline,supportingDocDeadline,applicationFee,IELTS,TOEFL,IB_english_test,iletsScore,toeflOnlineScore,toeflPaperScore,IBEnglishScore,oalevel,otherGrade,specialRequirement,IB_degree,american,septemberIntake,JanuaryIntake,JuneIntake,name,province,description,city,country FROM `institute_program` INNER JOIN `institute` ON institute_program.institute_id = institute.id WHERE program_id='"+req.params.program+"' AND "+req.params.term+" =1";   
           db.query(query, (err, Pdata) => {
            console.log(Pdata)
            if (err) {
              res.redirect('/search')
            }
            
            if(Pdata.length > 0){
                res.render('search_result2.ejs', {
                title: "Welcome to Socka | View Players",Pdata:Pdata,error:""
               
            })
            }else{
               console.log("g")
           
            }
               
          
    })

    }),
    router.get('/search-result/:search',(req,res)=>{
          let query = "SELECT * FROM `institute` WHERE city='"+req.params.search+"' OR type='"+req.params.search+"' OR province='"+req.params.search+"' OR country='"+req.params.search+"' OR name='"+req.params.search+"'"; 
           db.query(query, (err, Pdata) => {
            console.log(Pdata)
            if (err) {
              res.redirect('/search')
            }
            
            if(Pdata.length > 0){
                res.render('search_result.ejs', {
                title: "Welcome to Socka | View Players",Pdata:Pdata,error:""
               
            })
            }else{
                 res.render('search_result.ejs', {
                error:"No result found"
            })
            }
               
          
    })

    }),
router.post('/edit-aboutus',(req,res)=>{
  console.log(req.body)  
let query = "UPDATE `aboutuspage` SET `mission` = '"+req.body.mission+"',`plan` = '"+req.body.plan +"',`vision` = '"+req.body.vision+"',`why_choose_us` = '"+req.body.whoweare+"' "
            
               
                        db.query(query, (err, result) => {
                            if (err) {
                                return res.status(500).send(err);
                            }
                            res.redirect('/edit-aboutus');    
           
           
                        })
        
  
})
router.post('/edit-testimonial',(req,res)=>{
  console.log(req.body)  

            
                 let query = "INSERT INTO `testimonial` (person_name,institute,statement) VALUES ('" +req.body.name + "','" +req.body.institute + "', '" + req.body.statement + "')";
                        db.query(query, (err, result) => {
                            if (err) {
                                return res.status(500).send(err);
                            }
                            res.redirect('/edit-testimonial');    
           
           
                        })
        
  
})
router.get('/update-university-point/:id',(req,res)=>{
  res.render('temporary.ejs')                                  
})
router.get('/update-how-it-works/:id',(req,res)=>{
  res.render('temporary.ejs')                                  
})
router.get('/update-our-service/:id',(req,res)=>{
  res.render('temporary.ejs')                                  
})
router.get('/update-college-point/:id',(req,res)=>{
  res.render('temporary.ejs')                                  
})
router.get('/update-testimonial/:id',(req,res)=>{
  res.render('temporary.ejs')                                  
})
router.post('/update-college-point/:id',(req,res)=>{
  console.log("l",req.params.id)
      let query = "UPDATE `study_in_college` SET `point` = '"+req.body.unidata+"' WHERE `col_id` = '"+req.params.id +"'"
      db.query(query, (err, result) => {
                            if (err) {
                                return res.status(500).send(err);
                            }
                          res.redirect('/edit-college-in-canada');
            })
     
                     
              
})
router.post('/update-university-point/:id',(req,res)=>{
  console.log("l",req.params.id)
      let query = "UPDATE `study_in_university` SET `point` = '"+req.body.unidata+"' WHERE `uni_id` = '"+req.params.id +"'"
      db.query(query, (err, result) => {
                            if (err) {
                                return res.status(500).send(err);
                            }
                          res.redirect('/edit-university-in-canada');
            })
     
                     
              
})
router.post('/update-how-it-work/:id',(req,res)=>{
  console.log("l",req.params.id)
      let query = "UPDATE `how_it_works` SET `heading` = '"+req.body.hwheading+"',`description` = '"+req.body.hwdescription+"',`icon` = '"+req.body.hwicon+"' WHERE `id` = '"+req.params.id +"'"
      db.query(query, (err, result) => {
                            if (err) {
                                return res.status(500).send(err);
                            }
                          res.redirect('/edit-how-it-work');
            })
     
                     
              
})
router.post('/update-testimonial/:id',(req,res)=>{
  console.log("l",req.params.id)
      let query = "UPDATE `testimonial` SET `person_name` = '"+req.body.tsname+"',`institute` = '"+req.body.tsinstitute+"',`statement` = '"+req.body.tsstatement+"' WHERE `id` = '"+req.params.id +"'"
      db.query(query, (err, result) => {
                            if (err) {
                                return res.status(500).send(err);
                            }
                          res.redirect('/edit-testimonial');
            })
     
                     
              
})
router.post('/update-our-service/:id',(req,res)=>{
  console.log("l",req.params.id)
      let query = "UPDATE `our_services` SET `heading` = '"+req.body.editheading+"',`service` = '"+req.body.editservice+"',`icon` = '"+req.body.editicon+"' WHERE `id` = '"+req.params.id +"'"
      db.query(query, (err, result) => {
                            if (err) {
                                return res.status(500).send(err);
                            }
                          res.redirect('/edit-our-service');
            })
     
                     
              
})
router.post('/aboutus-description',(req,res)=>{
  console.log(req.body)  
// "UPDATE `aboutus` SET `main_one_heading`=",`main_two_heading`=[value-2],`main_three_heading`=[value-3],`main_one_desc`=[value-4],`main_two_desc`=[value-5],`main_three_desc`=[value-6],`study_canada_hone`=[value-7],`study_canada_htwo`=[value-8],`study_canada_hthree`=[value-9],`study_canada_hfour`=[value-10],`study_canada_done`=[value-11],`study_canada_dtwo`=[value-12],`study_canada_dthree`=[value-13],`study_canada_dfour`=[value-14] "
                 let query = "UPDATE `aboutus` SET `main_one_heading` = '"+req.body.mainheadingone + "',`main_two_heading` = '"+req.body.mainheadingtwo + "',`main_three_heading` = '"+req.body.mainheadingthree + "',`main_one_desc` = '"+req.body.maindescriptionone + "',`main_two_desc` = '"+req.body.maindescriptiontwo + "',`main_three_desc` = '"+req.body.maindescriptionthree + "',`study_canada_hone` = '"+req.body.headingone + "',`study_canada_htwo` = '"+req.body.headingtwo + "',`study_canada_hthree` = '"+req.body.headingthree + "',`study_canada_hfour` = '"+req.body.headingfour + "',`study_canada_done` = '"+req.body.descriptionone + "',`study_canada_dtwo` = '"+req.body.descriptiontwo + "',`study_canada_dthree` = '"+req.body.descriptionthree + "',`study_canada_dfour` = '"+req.body.descriptionfour + "'"
                        db.query(query, (err, result) => {
                            if (err) {
                                return res.status(500).send(err);
                            }
                            res.redirect('/aboutus-description');    
            })
                
})
     
                    
       



router.post('/add-institute',(req,res)=>{
  console.log(req.body)  
    let existquery = "SELECT * FROM `institute` WHERE name='"+req.body.institutename+"'"; // query database to get all the players
//
        // execute query
        db.query(existquery, (err, result) => {
            if (err) {
                res.redirect('/');
            }
            if(result.length > 0){
                console.log("result",result)
               res.render('add_institute.ejs', {
                title: "Welcome to Socka | View Players"
                ,message: 'University with this name already exists'
                
            });  
            }
            else{
                if(req.body.name != ""){
                 let query = "INSERT INTO `institute` (name, type,description,address,province,city,postalcode,country,videolink) VALUES ('" +req.body.institutename + "', '" + req.body.institutetype  + "', '" +req.body.description + "', '"+req.body.address+ "', '"+req.body.province + "','"+req.body.city +"','"+req.body.postalcode +"','"+req.body.country +"', '"+ req.body.videolink +  "')";
                        db.query(query, (err, result) => {
                            if (err) {
                                return res.status(500).send(err);
                            }
                            res.redirect('/add-institute?success="true"')
                            });
                 
            }
            }
        })
           fs.mkdir('/public', 0777,function (err) {
    if (err) console.error(err)
    else console.log('pow!')
});     
           
        });

router.post('/add-institute-program',(req,res)=>{
  console.log(req.body)  
    // query database to get all the players
//
   
    


         let existquery = "SELECT id FROM `institute` WHERE name='"+req.body.institutes+"'"; 
    db.query(existquery, (err, instituteID) => {
         
            if(instituteID.length > 0){
               
            }
            
          let existquery = "SELECT id FROM `programs` WHERE program_name='"+req.body.program+"'"; 
    db.query(existquery, (err, programID) => {
       
            if(programID.length > 0){
                
             
            }
  
   
   var i =0
       let newquery = "SELECT * FROM `institute_program` WHERE institute_id='"+instituteID[0].id+"' and program_id='"+programID[0].id+"'"; 
    db.query(newquery, (err, result3) => {
          if(result3.length > 0){
                           let existsquery = "SELECT name FROM `institute` "; // query database 
     let programquery = "SELECT program_name FROM `programs` "; // query database 
    db.query(existsquery, (err, result12) => {
            if (err) {
                res.redirect('/');
            }
           db.query(programquery, (err, result22) => {
            if (err) {
                res.redirect('/');
            }
          
            res.render('add_program_for_institute.ejs', {
                title: "Welcome to Socka | View Players",result:result12,result2:result22,message:"An Institute with this area of study arleady exists"
                
            })
           })
    })  
       
          }
        
            else{
                var b = instituteID
                 console.log("inst",instituteID[0].id)
   console.log("inst",programID[0].id)
                 let query = "INSERT INTO `institute_program` (institute_id,program_id,programRanking,OUACregistration,applicationDeadline,supportingDocDeadline,tuitionFees,applicationFee,septemberIntake,JanuaryIntake,JuneIntake,IELTS,TOEFL,IB_english_test,iletsScore,toeflOnlineScore,toeflPaperScore,IBEnglishScore,oalevel,otherGrade,specialRequirement,IB_degree,american,noenglishexam) VALUES ('" +instituteID[0].id + "', '" + programID[0].id + "', '" +req.body.ranking + "', '" +req.body.ouacregistration + "', '" +req.body.appdeadline + "', '" +req.body.docdeadline + "', '" +req.body.tuitionfee + "', '" +req.body.appfee + "', '" +req.body.sepintake + "', '" +req.body.janintake + "','"+req.body.juneintake+ "', '"+req.body.ielts + "','"+req.body.toefl +"','"+req.body.ibenglish +"', '" +req.body.ieltsscore + "', '" +req.body.toeflinternetscore + "', '" +req.body.toeflpaperscore + "', '" +req.body.ibenglishscore + "', '" +req.body.oalevel + "', '" +req.body.othergrade + "', '" +req.body.specialreq + "', '" +req.body.ibdegree + "', '" +req.body.american + "', '"+ req.body.noexam +  "')";
                        db.query(query, (err, result) => {
                            if (err) {
                                return res.status(500).send(err);
                            }
                            res.redirect('/add-institute-program?success="true"')
                            });
                 
            }
            
              
           
        });
         })
        })
})

router.post('/add-program',(req,res)=>{
  console.log(req.body)  
    let existquery = "SELECT * FROM `programs` WHERE program_name='"+req.body.programname+"'"; // query database to get all the players
//
        // execute query
        db.query(existquery, (err, result) => {
            if (err) {
                res.redirect('/');
            }
            if(result.length > 0){
                console.log("result",result)
               res.render('add_program.ejs', {
                title: "Welcome to Socka | View Players"
                ,message: 'Program with this name already exists'
                
            });  
            }
            else{
                if(req.body.name != ""){
                 let query = "INSERT INTO `programs` (program_name, tags,description,fouryearDegree,threeyeardegree,twoyearDegree) VALUES ('" +req.body.programname + "', '" +req.body.tags + "',  '" +req.body.description + "',  '" +req.body.fouryear + "', '" +req.body.threeyear + "',    '"+ req.body.twoyear +  "')";
                        db.query(query, (err, result) => {
                            if (err) {
                                return res.status(500).send(err);
                            }
                            res.redirect('/add-program?success="true"')
                            });
                 
            }
            }
        })
                 
           
        });



router.post('/eligibility-form/:result',(req,res)=>{
  console.log(req.body)  
//
       
let query = "INSERT INTO `form` ( name,phone,email,nationality,country_of_education,education_level,english_exam,ielts_score,toefl_paper_score,toefl_online_score,Area_of_interest,interested_program,message) VALUES ('" +req.body.name + "', '" +req.body.phone + "',  '" +req.body.email + "','" +req.body.nationality + "',  '" +req.body.countryofeducation + "', '" +req.body.highereducation + "', '" +req.body.englishexam + "','" +req.body.ielts + "','" +req.body.toeflpaper + "','" +req.body.toeflonline + "','" +req.body.areaofinterest + "','" +req.body.interestedprogram + "',    '"+ req.body.message +  "')";
            db.query(query, (err, result) => {
                    if (err) {
                      return res.status(500).send(err);
                            }
                        res.redirect('/eligibility-form/success')
                            });
                 
            })
            
       
                 
           
 

//below code for handling images 

router.post('/api/photo',function(req,res){
    var filename = ""
    var storage	=	multer.diskStorage({
  destination: function (req, file, callback) {
    callback(null, path.join('./public'));
  },
  filename: function (req, file, callback) {
      console.log(req.body)
    callback(null, file.originalname);
      filename =file.fieldname
  }
});
    console.log(filename)
var upload = multer({ storage : storage }).array('userPhotos',2);

	upload(req,res,function(err) {
		console.log(req.body);
		console.log(req.files);
		if(err) {
			return res.end("Error uploading file.");
		}
		res.end("File is uploaded");
	});
});


module.exports = router

