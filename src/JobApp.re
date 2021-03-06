open Utilities;

type resume = Services.resume;

type state = {
  url: string,
  company: string,
  position: string,
  postedDate: string,
  deadline: string,
  resumes: array(resume),
  resumeValue: string,
  status: string,
  error: bool,
  success: bool,
  loading: bool,
  errorCause: string,
};

type action =
  | UpdateUrl(string)
  | UpdateCompany(string)
  | UpdatePosition(string)
  | UpdatePostedDate(string)
  | UpdateDeadline(string)
  | UpdateResumes(array(resume))
  | UpdateResumeValue(string)
  | UpdateStatus(string)
  | Submit
  | SuccesfulSubmit
  | FailedSubmit(string);

let reducer = (action, state) =>
  switch (action) {
  | UpdateUrl(value) => ReasonReact.Update({...state, url: value})
  | UpdateCompany(value) => ReasonReact.Update({...state, company: value})
  | UpdatePosition(value) => ReasonReact.Update({...state, position: value})
  | UpdatePostedDate(value) =>
    ReasonReact.Update({...state, postedDate: value})
  | UpdateDeadline(value) => ReasonReact.Update({...state, deadline: value})
  | UpdateResumes(value) => ReasonReact.Update({...state, resumes: value})
  | UpdateResumeValue(value) =>
    ReasonReact.Update({...state, resumeValue: value})
  | UpdateStatus(value) => ReasonReact.Update({...state, status: value})
  | Submit =>
    ReasonReact.UpdateWithSideEffects(
      {...state, loading: true, error: false},
      _self => (),
    )
  | SuccesfulSubmit =>
    ReasonReact.Update({
      ...state,
      error: false,
      success: true,
      loading: false,
    })
  | FailedSubmit(why) =>
    ReasonReact.Update({
      ...state,
      error: true,
      success: false,
      loading: false,
      errorCause: why,
    })
  };

let component = ReasonReact.reducerComponent("JobApp");

let make = _children => {
  ...component,
  reducer,
  initialState: () => {
    url: "",
    company: "",
    position: "",
    postedDate: "",
    deadline: "",
    resumes: [||],
    resumeValue: "",
    status: "",
    error: false,
    success: false,
    loading: false,
    errorCause: "",
  },
  render: self => {
    /** Event handlers which function as sort of dispatchers */
    let changeUrl = x => UpdateUrl(x) |> self.send;
    let changeCompany = x => UpdateCompany(x) |> self.send;
    let changePosition = x => UpdatePosition(x) |> self.send;
    let changePostedDate = x => UpdatePostedDate(x) |> self.send;
    let changeDeadline = x => UpdateDeadline(x) |> self.send;
    let changeResumeValue = x => UpdateResumeValue(x) |> self.send;
    let changeStatusValue = x => UpdateStatus(x) |> self.send;
    let errorMessage =
      self.state.error
        ? "Failed to add application: " ++ self.state.errorCause : "";
    let successMessage = self.state.success ? "Added application" : "";
    self.state.loading
      ? <div className="jobapp-size"> <div className="loader" /> </div>
      : <div>
          <form
            onSubmit={ev => {
              ev->ReactEvent.Form.preventDefault;
              self.send(Submit);
            }}>
            <p className="error-message"> {errorMessage |> str} </p>
            <p className="success-message"> {successMessage |> str} </p>
            <input
              type_="text"
              name="company"
              placeholder="company"
              value={self.state.company}
              required=true
              /*** Ideally, I would've passed a separate prop such as a "dispatcher" function,
                   but honestly for the complexity of our app, I wouldn't bother  */
              onChange={evt => Utilities.valueFromEvent(evt) |> changeCompany}
            />
            <input
              type_="text"
              name="position"
              placeholder="position"
              value={self.state.position}
              required=true
              /*** Ideally, I would've passed a separate prop such as a "dispatcher" function,
                   but honestly for the complexity of our app, I wouldn't bother  */
              onChange={evt =>
                Utilities.valueFromEvent(evt) |> changePosition
              }
            />
            <ScrapingInput
              script=ScrapingFunctions.scriptUrl
              typeValue="url"
              validationFn=ScrapingFunctions.validateUrl
              processFn=ScrapingFunctions.toStringProcess
              reducerFn=changeUrl
              name="url"
              placeholder="url"
              value={self.state.url}
              required=true
            />
            <div className="form-horizontal-separator">
              <label> {"Date posted" |> str} </label>
              <ScrapingInput
                script=ScrapingFunctions.scriptHtmlBody
                typeValue="date"
                validationFn=ScrapingFunctions.validateDate
                processFn=ScrapingFunctions.extractPostedDateProcess
                reducerFn=changePostedDate
                name="postedDate"
                placeholder=""
                value={self.state.postedDate}
                required=true
              />
            </div>
            <div className="form-horizontal-separator">
              <label> {"Deadline" |> str} </label>
              <input
                type_="date"
                name="deadline"
                value={self.state.deadline}
                required=true
                onChange={ev =>
                  ev |> Utilities.valueFromEvent |> changeDeadline
                }
              />
            </div>
            <div className="form-horizontal-separator">
              <label>
                <input
                  type_="radio"
                  name="status"
                  value="Applied"
                  required=true
                  onChange={ev => valueFromEvent(ev) |> changeStatusValue}
                />
                {"Applied" |> str}
              </label>
              <label>
                <input
                  type_="radio"
                  name="status"
                  value="To apply"
                  onChange={ev => valueFromEvent(ev) |> changeStatusValue}
                />
                {"To apply" |> str}
              </label>
            </div>
            <select
              id="resumes"
              value={self.state.resumeValue}
              required=true
              onChange={evt =>
                Utilities.valueFromEvent(evt) |> changeResumeValue
              }>
              <option key="" value=""> {"CV used" |> str} </option>
              /*{ReasonReact.arrayToElement(*/
              {ReasonReact.array(
                 self.state.resumes
                 |> Array.map((el: resume) =>
                      <option key={el.id} value={el.id}>
                        {el.title ++ " " ++ el.revision |> str}
                      </option>
                    ),
               )}
            </select>
            <button className="btn submit-btn"> {"Submit" |> str} </button>
          </form>
        </div>;
  },
};
