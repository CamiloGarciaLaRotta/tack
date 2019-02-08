type state = {
  url: string,
  company: string,
  position: string,
};

type action =
  | UpdateUrl(string)
  | UpdateCompany(string)
  | UpdatePosition(string);

let reducer = (action, _state) =>
  switch (action) {
  | UpdateUrl(value) => ReasonReact.Update({..._state, url: value})
  | UpdateCompany(value) => ReasonReact.Update({..._state, company: value})
  | UpdatePosition(value) => ReasonReact.Update({..._state, position: value})
  };

let component = ReasonReact.reducerComponent("JobApp");

let make = (~submitHandler, ~signOutHandler, _children) => {
  ...component, /* spread the template's other defaults into here  */
  reducer,
  initialState: () => {url: "", company: "", position: ""},
  render: self => {
    /** Event handlers which function as sort of dispatchers */
    let changeUrl = x => self.send(UpdateUrl(x));
    let changeCompany = x => self.send(UpdateCompany(x));
    let changePosition = x => self.send(UpdatePosition(x));
    <div>
      <form>
        <ScrapingInput
          script=ScrapingFunctions.scriptCompany
          typeValue="text"
          validationFn=ScrapingFunctions.validateNonNull
          processFn=ScrapingFunctions.toStringProcess
          reducerFn=changeCompany
          name="company"
          placeholder="company"
          value=self.state.company
        />
        <ScrapingInput
          script=ScrapingFunctions.scriptPosition
          typeValue="text"
          validationFn=ScrapingFunctions.validateNonNull
          processFn=ScrapingFunctions.toStringProcess
          reducerFn=changePosition
          name="position"
          placeholder="position"
          value=self.state.position
        />
        <ScrapingInput
          script=ScrapingFunctions.scriptUrl
          typeValue="url"
          validationFn=ScrapingFunctions.validateUrl
          processFn=ScrapingFunctions.toStringProcess
          reducerFn=changeUrl
          name="url"
          placeholder="url"
          value=self.state.url
        />
        <div className="form-horizontal-separator">
          <label> (ReasonReact.stringToElement("Date posted")) </label>
          <input _type="date" name="dateposted" />
        </div>
        <div className="form-horizontal-separator">
          <label> (ReasonReact.stringToElement("Deadline")) </label>
          <input _type="date" name="deadline" />
        </div>
        <div className="form-horizontal-separator">
          <label>
            <input _type="radio" name="status" value="applied" />
            (ReasonReact.stringToElement("Applied"))
          </label>
          <label>
            <input _type="radio" name="status" value="toApply" />
            (ReasonReact.stringToElement("To apply"))
          </label>
        </div>
        <button className="btn submit-btn" onClick=submitHandler>
          (ReasonReact.stringToElement("Submit"))
        </button>
        <span className="form-vertical-separator">
          <p className="form-vertical-separator-txt">
            (ReasonReact.stringToElement("or"))
          </p>
        </span>
        <button className="btn signout-btn" onClick=signOutHandler>
          (ReasonReact.stringToElement("Sign Out"))
        </button>
      </form>
    </div>;
  },
};